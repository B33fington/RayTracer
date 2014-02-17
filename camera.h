#ifndef _CAMERA_H
#define _CAMERA_H

#include "precompiled.h"

class Camera {
	vec3 camPos, camDir, camRight, camDown;

	public: 

	Camera();
	Camera(vec3, vec3, vec3, vec3);

	//method functions

	vec3 getCameraPosition() {return camPos;}
	vec3 getCameraDirection() {return camDir;}
	vec3 getCameraRight() {return camRight;}
	vec3 getCameraDown() {return camDown;}
};

Camera::Camera() 
{
	camPos = vec3(0,0,0);
	camDir = vec3(0,0,1);
	camRight = vec3(0,0,0);
	camDown = vec3(0,0,0);
}

Camera::Camera(vec3 pos, vec3 dir, vec3 right, vec3 down)
{
	camPos = pos;	
	camDir = dir;
	camRight = right;
	camDown = down;
}
#endif