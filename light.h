#ifndef _LIGHT_H
#define _LIGHT_H

#include "precompiled.h"

class Light : public Source {
	vec3 position;
	Color color;

	public: 

	Light ();

	Light(vec3, Color);

	//method functions

	virtual vec3 getLightPosition() {return position;}
	virtual Color getLightColor() {return color;}
};

Light::Light() 
{
	position = vec3(0,0,0);
	color = Color(1,1,1,0);
}

Light::Light (vec3 p, Color c)
{
	position = p;	
	color = c;
}
#endif