#ifndef _VEC3_H
#define _VEC3_H

#include "precompiled.h"

class vec3 {
	
	double x, y, z;

	public:
	vec3 ();

	vec3(double, double, double);

	//method functions

	double getX() {return x;}
	double getY() {return y;}
	double getZ() {return z;}

	double magnitude(); 
	double dot(vec3);

	vec3 normalize();
	vec3 invert();
	vec3 cross(vec3);
	vec3 add(vec3);
	vec3 mult(double);

};

vec3::vec3() 
{
	x = 0;
	y = 0;
	z = 0;
}

vec3::vec3 (double a, double b, double c)
{
	x = a;
	y = b;
	z = c;
}

double vec3::magnitude()
{
	return sqrt((x*x) + (y*y) + (z*z));
}

double vec3::dot(vec3 v)
{ 
	return x*v.getX() + y*v.getY() + z*v.getZ();
}

vec3 vec3::normalize()
{
	double magnitude = sqrt((x*x) + (y*y) + (z*z));
	return vec3(x/magnitude, y/magnitude, z/magnitude);
}

vec3 vec3::invert()
{
	return vec3(-x, -y, -z); 
}

vec3 vec3::cross(vec3 v)
{
	vec3 temp = vec3(y*v.getZ() - z*v.getY(),
				     z*v.getX() - x*v.getZ(),
					 x*v.getY() - y*v.getX());
	return temp;
}

vec3 vec3::add(vec3 v)
{
	return vec3(x + v.getX(), y + v.getY(), z + v.getZ());
}

vec3 vec3::mult(double f)
{
	return vec3(x*f, y*f, z*f);
}

#endif
