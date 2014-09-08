#pragma strict

import System;
import System.Threading;
import System.Net;
import System.Net.Sockets;

//---------------------------------------------------------

// the network client thread
private var thread : Thread = null;

// the server address
private var serverIP :IPAddress;

// the network port to use
private var networkPort :ushort = 3010;

// is the client running?
private var running = false;

// callback function used to notify when data has arrived
private var dataCallback :function(TrackerData) = null;

//---------------------------------------------------------

// 6DOF tracking data received from the tracking server
class TrackerData {
	var timeStamp :float;		// time stamp of position data
	var sensor :int;			// sensor number
	var position :Vector3;		// position of the sensor
	var rotation :Quaternion;	// orientation of the sensor
	var valid :boolean;			// is the data valid?
	
	function TrackerData() {
		timeStamp = 0;
		sensor = 0;
		position = Vector3(0,0,0);
		rotation = Quaternion.identity;
		valid = false;
	}
	
	// convert tracker data to a 4x4 matrix
	function toMatrix4x4() {
		return Matrix4x4.TRS(
			this.position,
			this.rotation,
			Vector3(1,1,1)	// scale: always 1
		);
	}
};

//---------------------------------------------------------

// start the tracker client
function start(
	callback :function(TrackerData),	// callback to receive data
	server :String,						// network server name
	port :ushort						// network port number
) {
    Debug.Log( "Client: start" );
    
    // install the user defined data callback
    dataCallback = callback;
    
    // get server host address
    serverIP = Dns.GetHostAddresses(server)[0];
    Debug.Log( "Client: server=" + server + "(" + serverIP + ")" );
    
    // store the network port
    networkPort = port;
    
    // start the network client thread
    thread = new Thread( threadListener );
    thread.Start();
}

//---------------------------------------------------------

// stop the tracker client
function stop() {
	Debug.Log( "Client: stop" );
	
	// ask the thread to stop
	running = false;
	
	// wait until the thread stops
	if (thread != null) {
	    thread.Join(2500);
	    thread = null;
    }
}

//---------------------------------------------------------

// the network client thread
private function threadListener() {
	Debug.Log( "Client: thread started" );
	
    var client = new TcpClient();
    var stream : NetworkStream;
    var port = networkPort;

    try {
    	Debug.Log( "Client: trying to connect" );
        client.Connect( IPEndPoint(serverIP, port) );

        stream = client.GetStream();

		Debug.Log( "Client: connected" );

        // receive buffer
        var data = new byte[36];

        // wait for data
        var ticks = 1000;
        while ( !stream.DataAvailable && (ticks > 0) ) {
            Thread.Sleep(10);
            ticks--;
        }

        // have we timed out?
        if ( ticks == 0 ) {
            // 10053 is the Windows Socket error code for when software causes a connection abort
            throw SocketException( 10053 );
        }
        Debug.Log( "Client: received");

        running = true;
        
        // we have received data
        while (running) {
            // read data
            stream.Read( data, 0, data.Length );

			var trackData :TrackerData = new TrackerData();
			
			// time in seconds
			trackData.timeStamp = BitConverter.ToSingle( data, 0 );
			
			// sensor identifier number
			trackData.sensor = BitConverter.ToInt32( data, 4 );
			
			// position vector
			trackData.position.x = BitConverter.ToSingle( data,  8 );
			trackData.position.y = BitConverter.ToSingle( data, 12 );
			trackData.position.z = BitConverter.ToSingle( data, 16 );
			
			// orientation quaternion
			trackData.rotation.x = BitConverter.ToSingle( data, 20 );
			trackData.rotation.y = BitConverter.ToSingle( data, 24 );
			trackData.rotation.z = BitConverter.ToSingle( data, 28 );
			trackData.rotation.w = BitConverter.ToSingle( data, 32 );
			
			// mark the data as valid
			trackData.valid = true;
		
			dataCallback( trackData );
        }
    }
    catch (InvOpEx : InvalidOperationException) {
        Debug.Log( "TCP exception: " + InvOpEx.Message );
    }
    catch (SockEx : SocketException) {
        Debug.Log( "Socket exception: " + SockEx.Message );
    }
    finally {
        if ( stream != null )
            stream.Close();
        // todo: how to gracefully disconnect in .net?
        //client.Client.Shutdown( SocketShutdown.Both );
        client.Close();
    }
}

//---------------------------------------------------------
