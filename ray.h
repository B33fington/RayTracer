#ifndef _RAY_H
#define _RAY_H

#include "precompiled.h"

class Ray {
	vec3 origin, direction;

	public: 

	Ray ();

	Ray(vec3, vec3);

	//method functions

	vec3 getRayOrigin() {return origin;}
	vec3 getRayDirection() {return direction;}
};

Ray::Ray() 
{
	origin = vec3(0,0,0);
	direction = vec3(1,0,0);
}

Ray::Ray (vec3 o, vec3 d)
{
	origin = o;	
	direction = d;
}
#endif