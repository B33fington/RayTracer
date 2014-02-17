#ifndef _OBJECT_H
#define _OBJECT_H

#include "precompiled.h"

class Object {

	public: 

	Object ();

	//method functions
	virtual Color getColor() {return Color(0,0,0,0);}
	virtual vec3 getNormalAt(vec3) {return vec3(0,0,0);}
	virtual double findIntersection(Ray r) {return 0;}
};

Object::Object() {}

#endif
