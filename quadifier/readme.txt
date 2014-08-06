Quadifier
---------

This software was developed to allow Unity 3D to be run with quad-buffer OpenGL
stereo. It requires some scripts on the Unity side to render the left and right
channels in the right order. It works on the free version of Unity and doesn't
require Pro.

There are separate implementations for Windows and Linux, although there
is a small amount of shared code between the two.

J.Ward, 20/12/13

Building for Windows
--------------------
Enter the win32 folder and build using the Visual Studio Solution

Building for Linux
------------------
Enter the linux folder and type make
