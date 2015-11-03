# quadifier
OpenGL Quad Buffer Stereo 3D for the Unity Game Engine (and potentially others)

This software was developed to allow Unity 3D to be run with quad-buffer OpenGL
stereo. It works by intercepting DirectX, and renders this back out as a stereo
pair using OpenGL. This is achieved using the NVIDIA OpenGL/DirectX interop.

Quadifier requires some scripts on the Unity side to render the left and right
channels in the right order. It works on the free version of Unity and doesn't
require Pro.

There are separate implementations for Windows and Linux, although there
is a small amount of shared code between the two.

Further documentation is available in the source tree and in the Github Wiki.

J.Ward 03/11/15
