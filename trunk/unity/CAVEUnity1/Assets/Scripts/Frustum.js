#pragma strict

//---------------------------------------------------------

/// returns a perspective projection matrix
public static function computeFrustum (
	left 	:double,
	right 	:double,
	bottom	:double,	
	top 	:double,
	near 	:double,
	far 	:double
) {
	var a = (right + left) / (right - left);
	var b = (top + bottom) / (top - bottom);
	var c = -(far + near) / (far - near);
	var d = -2 * near * far / (far - near);
	
	var x = 2 * near / (right - left);
	var y = 2 * near / (top - bottom);
	
	var m : Matrix4x4;
	
	m[0,0] = x; m[0,1] = 0; m[0,2] = a; m[0,3] = 0;
	m[1,0] = 0; m[1,1] = y; m[1,2] = b; m[1,3] = 0;
	m[2,0] = 0; m[2,1] = 0; m[2,2] = c; m[2,3] = d;
	m[3,0] = 0; m[3,1] = 0; m[3,2] =-1; m[3,3] = 0;
	
	return m;
}//computeFrustum

//---------------------------------------------------------

/// set the camera frustum (using the existing near and far clipping planes)
public static function setCameraFrustum(
	camera 	:Camera,
	left 	:double,
	right 	:double,
	bottom	:double,
	top 	:double
) {
 	var m : Matrix4x4 = computeFrustum(
		left, right, bottom, top,
		camera.nearClipPlane,
		camera.farClipPlane
	);
	camera.projectionMatrix = m;
}//setCameraFrustum

//---------------------------------------------------------

/// used to test whether the frustum() function works
public static function testCameraFrustum(
	camera :Camera
) {
	// get current vertical field of view of camera
	var fov = Mathf.Deg2Rad * camera.fieldOfView;
	
	// calculate half width and height at near plane, using
	// the camera nearClipPlane, fov and aspect ratio
	var halfHeight = camera.nearClipPlane * Mathf.Tan( fov / 2 );
	var halfWidth  = camera.aspect * halfHeight;
	
	// set up camera frustum: this should create the
	// same result as the existing Unity camera
	setCameraFrustum(
		camera,
		- halfWidth,
		  halfWidth,
		- halfHeight,
		  halfHeight
	);
}//testCameraFrustum

//---------------------------------------------------------
