Quadifier
---------

This software was developed to allow Unity 3D to be run with quad-buffer OpenGL
stereo on Windows. Unity 3D is run in Direct3D 9 mode, and the rendered output
is captured and rendered back using OpenGL into the back left and back right
buffers. It requires some scripts on the Unity side to render the left and
right channels in the right order. It works on the free version of Unity
and doesn't require Pro.

Quadifier consists of two components:
* Module: this is a DLL which contains the main implementation.
* Launcher: this is an executable which launches the Unity executable and
injects the DLL into Unity, so that all rendering can be intercepted.

Launcher loads the Unity process, then creates a remote thread inside
the process and uses some inline assembler to load the DLL.

Module hooks several of the Direct3D functions, so that it can intercept any
calls to these functions. Hooking is achieved using the third-party mhook
library. This is included in the extern folder, so the project is fully self
contained.

When Unity tries to call Direct3DCreate9, this is redirected to a substitute
function. Instead of returning the real IDirect3D9 interface, it returns a
proxy called IDirect3D9Proxy which is a wrapper for the real IDirect3D9 class.

Similarly, the IDirect3D9Proxy::CreateDevice returns a proxy called
IDirect3D9Device9Proxy. Most of the class methods call straight into Direct3D,
but a few of them are intercepted and are used to capture significant events
such as Clear and Present. These are used to determine when rendering has
begun and ended for a particular frame, so that we can capture the results.

The left and right images are rendered sequentially in Direct3D, and are
captured and moved into the appropriate left/right buffers by Quadifier.

Scripts are used in Unity to render the left and right images, and to
create the appropriate asymmetric camera frustum needed for correct
stereo projection.

Most of the work is carried out by the Quadifier class. It causes Direct3D
to render into a render target instead of the screen. The corresponding
surface is mapped onto an OpenGL texture or renderbuffer using the NVIDIA
NV_DX_interop extension. This is attached to a framebuffer object. The
framebuffer object is blitted to the back left or back right OpenGL render
buffers as appropriate.

The OpenGL renderer creates a separate window, with the same multisample
format as the Direct3D window. This also accounts for situations where the
multisampling has been "forced on" in the NVIDIA Control Panel (it does
this by creating a temporary GL context at startup and testing whether it
is multisampled). This is important because the interop fails in some
cases if the source and destination multisampling don't match.

The windows messages received by the OpenGL window are forwarded on to
the Unity window so that mouse and keyboard work.

The OpenGL renderer runs on a separate thread and is loosely synchronised
with the Unity renderer via a critical section with a timeout.

When the software is run on a WDDM operating system (Vista, Win 7, Win 8),
the NV_DX_interop requires a IDirect3D9Ex and IDirect3DDevice9Ex context.
Unity doesn't create one of these by default, so Quadifier substitutes the
appropriate 3D9Ex interfaces at time of creation.

Another issue we have to deal with is that the D3DPOOL_MANAGED pool isn't
valid when using the Direct3D9Ex context. For this reason, whenever
D3DPOOL_MANAGED is used, IDirect3DDevice9Proxy translates it into
D3DPOOL_DEFAULT instead, and sets D3DUSAGE_DYNAMIC so that it can be locked.

There is partial implementation of support for Direct3D11, but more work
is needed on this as it's a lot more complex. In their wisdom, Microsoft
provide 6 different ways to create a swap chain and 2 present functions.
Many more proxy classes are needed with Direct3D11 to get at the calls
required, and it's easy to trip up on the reference counting.

Current status
--------------
This currently (R359) works on Windows 7 and Windows XP, having been tested
on about 5 different PCs to date. The following issues have been noticed:

* Only works on the CUBE if the mosaic is the primary monitor, and the
window must be wholly on the CUBE (otherwise stereo does not display)

James
16/10/13

