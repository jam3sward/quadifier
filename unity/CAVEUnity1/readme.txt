CAVEUnity1
----------

This is a test project for Unity 3D which supports:
* Multiple displays (number of screens, position, orientation and screen size is
read from an XML configuration file)
* Stereo rendering (requires Quadifer to launch Unity executable)
* Tracking (requires Unity VRPN bridge software)

There's a simple test scene, but everything is really done by scripting.
The scripts are as follows:

Quadifier.js
This is the main script which initialises all the cameras and handles the
head tracking updates.

Frustum.js
Allows the Unity camera frustum to be adjusted, so that an asymmetric frustum
can be created for correct stereo rendering.

TrackerClient.js
Implements a simple TCP client which connects over the network to a tracking server,
receives 6DOF tracking data and passes it into Unity. This can be used to implement
head, hand and instrument tracking in Unity. It requires a separate console
application which acts as a bridge between Unity and VRPN. This is because we use a
simple proprietary protocol (to avoid having to implement the full VRPN protocol in
script).

Serialize.js
XML object serialisation to/from file.

Ultimately, the plan is that a scene can be modelled in Unity at appropriate scale,
then you can drop the scripts into the project and run it on pretty much any screen
configuration with stereo and head tracking. Unity objects could also be attached
to tracked instruments etc.

James
12/09/13
