#pragma strict

import System.Xml;
import System.Collections.Generic;
import System.Threading;

//-----------------------------------------------------------------------------

private var serialize :Serialize;			// import Serialize.js script
private var frustum :Frustum;				// import Frustum.js script
private var trackerClient :TrackerClient;	// import TrackerClient.js script

//-----------------------------------------------------------------------------

// the name of the settings file, read at initialisation
private var settingsFile = "settings.xml";

// maximum number of tracked items
private var maxTrackedItems = 8;

// stores latest tracking data received
private var trackerData :TrackerData[];

// stores references to tracked objects in scene
private var trackedItem :GameObject[];

// lock to control access to the tracker data array
private var trackerLock = new Object();

//-----------------------------------------------------------------------------

// represents the parameters of a projection screen
class ProjectionScreen {
	var name	 :String;		// name of screen
	var centre   :Vector3;		// world coordinates of screen centre
	var normal   :Vector3;		// the screen normal vector
	var up 		 :Vector3;		// the screen up vector
	var width	 :float;		// width of screen in world coordinates
	var height	 :float;		// height of screen in world coordinates
	var viewport :Rect;			// normalised viewport rectangle for rendering
	
	// constructor
	function ProjectionScreen() {
		name = "ProjectionScreen";
		width = 1;
		height = 1;
		viewport = Rect(0,0,1,1);
	}
	
	// creates a camera for this screen (which is created as a game object in
	// the unity scene) and returns the camera for further adjustment
	function createCamera( label :String ) {
		// create a new game object in Unity
		var object = new GameObject();
		object.AddComponent( Camera );
		
		// set the object name
		object.name = "Camera (" + this.name + ")";
		if ( label != null ) object.name += " " + label;
		
		// get the camera
		var camera = object.camera;
		
		// set the camera transformation
		camera.transform.position = Vector3( 0, 0, 0 );
		camera.transform.LookAt( this.centre, this.up );
		
		// set the camera viewport on screen
		camera.rect = this.viewport;
		
		// return the camera
		return camera;
	}
};

//-----------------------------------------------------------------------------

// stores the settings (can be serialized to/from file)
class Settings
{
	// the projection screens
	public var screens :List.<ProjectionScreen>;
	
	// eye separation (inter-pupillary distance)
	public var eyeSeparation :float;

	// tracker network port number
	public var networkPort :ushort;
	
	// tracker transformation matrix
	public var trackerMatrix :Matrix4x4;
	
	// constructor: sets the default values
	function Settings() {
		eyeSeparation = 0.07;	// default eye separation
		networkPort   = 3010;	// default network port
	}
};

//-----------------------------------------------------------------------------

// stores a camera rig associated with a particular screen
class CameraRig {
	public var left	 :Camera;
	public var right :Camera;
	public var screen :ProjectionScreen;
	var frustum :Frustum;
	
	// creates a camera rig for a particular screen
	function CameraRig(theScreen :ProjectionScreen) {
		// store the screen parameters
		screen = theScreen;
		
		// create left camera
		left  = screen.createCamera("left");
		
		// create right camera
		right = screen.createCamera("right");
	}

	// update an individual camera	
	function updateCamera( camera :Camera, eye :Vector3 ) {
		// calculate distance from eye position to screen plane
		var plane = new Plane( this.screen.normal, this.screen.centre );		
		var viewDistance = Math.Abs( plane.GetDistanceToPoint( eye ) );

		// get the near clipping plane distance
		var near = camera.nearClipPlane;
		
		// position the camera at the eye point
		camera.transform.position = eye;
		
		// ensure the camera is facing the projection screen
		camera.transform.LookAt( eye - this.screen.normal, this.screen.up );

		// transform the eye from world to projection screen coordinates,
		// so that the X and Y values in setCameraFrustum represent the
		// horizontal and vertical position for that screen
		// note: the Z coordinate is ignored after transformation
		eye = Quaternion.Inverse( camera.transform.rotation ) * eye;
		
		// set the camera frustum based on eye position and viewing
		// distance from screen
		frustum.setCameraFrustum(
			camera,
			(-eye.x - 0.5f * this.screen.width) * near / viewDistance,
			(-eye.x + 0.5f * this.screen.width) * near / viewDistance,
			(-eye.y - 0.5f * this.screen.height)* near / viewDistance,
			(-eye.y + 0.5f * this.screen.height)* near / viewDistance
		);
	}
	
	// update the camera rig, given the eye positions
	function update( eyeLeft :Vector3, eyeRight :Vector3 ) {
		updateCamera( left,  eyeLeft  );
		updateCamera( right, eyeRight );
	}
};

//-----------------------------------------------------------------------------

// stores the settings
var settings :Settings = new Settings();

// stores a list of camera rigs (one rig per screen)
var cameras :List.<CameraRig>;

//-----------------------------------------------------------------------------

// create a set of screens to represent a four sided 2.5m cubed CAVE
function setupScreensForCAVE() {
	var screen :ProjectionScreen;
	
	// left
	screen = new ProjectionScreen();
	screen.name = "Left";
	screen.centre = Vector3(1.25,0,0);
	screen.normal = Vector3(-1,0,0);	// right
	screen.up	  = Vector3(0,1,0);		// up
	screen.width  = 2.5;
	screen.height = 2.5;
	screen.viewport = Rect(0,0,0.25,1);
	settings.screens.Add(screen);	
	
	// front
	screen = new ProjectionScreen();
	screen.name = "Front";
	screen.centre = Vector3(0,0,-1.25);
	screen.normal = Vector3(0,0,1);		// forward
	screen.up	  = Vector3(0,1,0);		// up
	screen.width  = 2.5;
	screen.height = 2.5;
	screen.viewport = Rect(0.25,0,0.25,1);
	settings.screens.Add(screen);
	
	// right
	screen = new ProjectionScreen();
	screen.name = "Right";
	screen.centre = Vector3(-1.25,0,0);
	screen.normal = Vector3(1,0,0);		// left
	screen.up	  = Vector3(0,1,0);		// up
	screen.width  = 2.5;
	screen.height = 2.5;
	screen.viewport = Rect(0.5,0,0.25,1);
	settings.screens.Add(screen);
	
	// floor
	screen = new ProjectionScreen();
	screen.name = "Floor";
	screen.centre = Vector3(0,1.25,0);
	screen.normal = Vector3(0,1,0);		// up
	screen.up	  = Vector3(1,0,0);		// left
	screen.width  = 2.5;
	screen.height = 2.5;
	screen.viewport = Rect(0.75,0,0.25,1);
	settings.screens.Add(screen);
}

//-----------------------------------------------------------------------------

// initialise the camera rig for each screen
function setupCameras() {
	// render our cameras after the main camera, and render the left channel
	// cameras before the right channel cameras
	var cameraDepthLeft  = Camera.main.depth + 1;
	var cameraDepthRight = cameraDepthLeft + 1;
	
	// disable the main camera (we only use cameras created from script)
	Camera.main.enabled = false;	
	
	// for each screen, create the corresponding camera
	for ( var screen in settings.screens ) {
		// create camera pair
		var rig = new CameraRig( screen );
		rig.left.depth  = cameraDepthLeft;
		rig.right.depth = cameraDepthRight;
		cameras.Add( rig );
		
		// set up temporary initial eye positions, assuming that the X axis
		// points left, and the user is facing the front screen on the -Z
		// axis of the scene
		rig.update(
			Vector3(  settings.eyeSeparation/2, 0, 0 ),
			Vector3( -settings.eyeSeparation/2, 0, 0 )
		);
	}
}

//-----------------------------------------------------------------------------

// this function is called when tracking data is received (from another thread)
function trackerDataCallback( data :TrackerData ) {
	// debugging: display data as received
	Debug.Log(
		  "t=" + data.timeStamp +
		", s=" + data.sensor 	+
		", p=" + data.position 	+
		", r=" + data.rotation
	);
	
	// store tracker data
	if ( (data.sensor >= 0) && (data.sensor < trackerData.Length) ) {
		// lock before writing to array
		Monitor.Enter( trackerLock );
			trackerData[data.sensor] = data;
		Monitor.Exit( trackerLock );
	}
}

//-----------------------------------------------------------------------------

// called at wake up
function Awake () {
	// render as fast as possible
	Application.targetFrameRate = -1;
}

//-----------------------------------------------------------------------------

// called at start up
function Start () {
	serialize = GetComponent( Serialize );
	frustum = GetComponent( Frustum );
	trackerClient = GetComponent( TrackerClient );	

	// allocate tracker data and tracked item builtin arrays
	trackerData = new TrackerData[maxTrackedItems];
	trackedItem = new GameObject[trackerData.Length];
	
	// save settings to file (useful to create an XML file with the proper
	// format, for later manual editing)
	//setupScreensForCAVE();
	//serialize.saveXML( settings, settingsFile );

	// load the settings from file
	settings = serialize.loadXML( settings, settingsFile ) as Settings;
	
	// initialise the cameras from settings
	setupCameras();
	
	// start the tracker client, and install a callback to receive data
	trackerClient.start( trackerDataCallback, "localhost", settings.networkPort );
}

//-----------------------------------------------------------------------------

// update position of all tracked objects
// any objects named "tracker0", "tracker1" etc. will be repositioned
function updateObjects () {
	// potentially quite inefficient to lock for the entire update loop
	Monitor.Enter( trackerLock );
	
	// for each object
	for (var i=0; i<trackerData.Length; i++) {
		// ignore invalid items
	    if ( (trackerData[i] == null) || !trackerData[i].valid ) continue;
	    
		// attempt to find (and cache reference to) corresponding object
	    if ( trackedItem[i] == null )
	    	trackedItem[i] = GameObject.Find( "tracker" + i );
	    
	    // update object position
	    if ( trackedItem[i] != null ) {
	    	trackedItem[i].transform.position = trackerData[i].position;
	    	trackedItem[i].transform.rotation = trackerData[i].rotation;
	    }
	}
	
	// unlock
	Monitor.Exit( trackerLock );
}

//-----------------------------------------------------------------------------

// update position of all cameras
function updateCameras () {
	// copy tracker data
	var tracker = new TrackerData();
	Monitor.Enter( trackerLock );
		if ( trackerData[0] != null )
			tracker = trackerData[0];
		else
			tracker.valid = false;
	Monitor.Exit( trackerLock );
	
	// exit if data is invalid
	if ( !tracker.valid ) return;

	// current head position and orientation is in 'tracker'
	// convert to 4x4 matrix
	var matrix :Matrix4x4 = tracker.toMatrix4x4();

	// transform the head position by the tracking matrix from tracking
	// coordinates to Unity coordinates
	// todo: these needs cleaning up, to read glasses transform from file also
	var eyeCentre :Vector3 = settings.trackerMatrix * matrix.GetColumn(3);
	var eyeOffset :Vector3 = (settings.trackerMatrix * matrix.GetColumn(0)) * settings.eyeSeparation / 2;
	var eyeLeft   :Vector3 = eyeCentre - eyeOffset;
	var eyeRight  :Vector3 = eyeCentre + eyeOffset;
	
	// update the position of all camera rigs
	for (var rig in cameras)
		rig.update( eyeLeft, eyeRight );
}

//-----------------------------------------------------------------------------

function Update () {
	// ensure vsync is disabled
	// note: calling this from Start doesn't seem to work (it changes back),
	// presumably because player quality settings are applied after start up?
	QualitySettings.vSyncCount = 0;

	// update camera positions
	updateCameras();
	
	// update object positions
	updateObjects();
}

//-----------------------------------------------------------------------------

function OnApplicationQuit() {
	// stop the tracker client before we exit
	trackerClient.stop();
}

//-----------------------------------------------------------------------------
