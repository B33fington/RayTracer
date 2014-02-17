#ifndef _SPHERE_H
#define _SPHERE_H

#include "precompiled.h"

class Sphere : public Object {
	vec3 center;
	double radius;
	Color color;

	public: 

	Sphere ();

	Sphere(vec3, double, Color);

	//method functions

	vec3 getSphereCenter() {return center;}
	double getSphereRadius() {return radius;}
	virtual Color getColor() {return color;}
	
	virtual vec3 getNormalAt(vec3 point){
		//normal always points away from the center of the Sphere
		vec3 normalVec = point.add(center.invert()).normalize();
		return normalVec;
	}

	virtual double findIntersection(Ray r){
		vec3 rayOrigin = r.getRayOrigin();
		double rayOriginX = rayOrigin.getX();
		double rayOriginY = rayOrigin.getY();
		double rayOriginZ = rayOrigin.getZ();

		vec3 rayDirection = r.getRayDirection();
		double rayDirectionX = rayDirection.getX();
		double rayDirectionY = rayDirection.getY();
		double rayDirectionZ = rayDirection.getZ();

		vec3 sphereCenter = center;
		double sphereCenterX = sphereCenter.getX();
		double sphereCenterY = sphereCenter.getY();
		double sphereCenterZ = sphereCenter.getZ();

		//double a = 1; //normalized
		double b = 2*(rayOriginX - sphereCenterX)*rayDirectionX + 2*(rayOriginY - sphereCenterY)*rayDirectionY + 2*(rayOriginZ - sphereCenterZ)*rayDirectionZ;
		double c = pow(rayOriginX - sphereCenterX,2) + pow(rayOriginY - sphereCenterY,2) + pow(rayOriginZ - sphereCenterZ,2) - radius*radius;

		double discriminant = b*b - 4*c;

		if(discriminant > 0) {
			//the ray intersects the Sphere
			//the first root
			double root1 = ((-1*b - sqrt(discriminant))/2) - ACCURACY; 
			if(root1 > 0){
				//the first root is smallest positive root
				return root1;
			} 
			else {
				//the second root is the smallest
				double root2 = ((sqrt(discriminant) - b)/2) - ACCURACY;
				return root2;
			}

		}
		else {
			// the ray missed the Sphere
			return -1;
		}
	}
};

Sphere::Sphere() 
{
	center = vec3(0,0,0);
	radius = 1.0;
	color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere (vec3 cent, double r, Color c)
{
	center = cent;	
	radius = r;
	color = c;
}
#endif