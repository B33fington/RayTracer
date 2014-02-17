#ifndef _PLANE_H
#define _PLANE_H

#include "precompiled.h"

class Plane : public Object {
	vec3 normal;
	double distance;
	Color color;

	public: 

	Plane ();

	Plane(vec3, double, Color);

	//method functions

	vec3 getNormal() {return normal;} 
	double getPlaneDistance() {return distance;}
	virtual Color getColor() {return color;}

	virtual vec3 getNormalAt(vec3 point){
		return normal;
	}

	virtual double findIntersection(Ray ray){
		vec3 rayDirection = ray.getRayDirection();

		double d = rayDirection.dot(normal);	

		if(d == 0){
			//ray is parallel to Plane
			return -1;
		} else { 
			double b = normal.dot(ray.getRayOrigin().add(normal.mult(distance).invert()));
			return -1 * b/d;
		}
	}
};

Plane::Plane() 
{
	normal = vec3(1,0,0);
	distance = 0.0;
	color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane (vec3 norm, double dist, Color c)
{
	normal = norm;	
	distance = dist;
	color = c;
}
#endif