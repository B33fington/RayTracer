#ifndef _COLOR_H
#define _COLOR_H

#include "precompiled.h"

class Color {
	
	double r, g, b, s;

	public:
	Color ();

	Color(double, double, double, double);

	//method functions

	double getRed() {return r;}
	double getGreen() {return g;}
	double getBlue() {return b;}
	double getSpec() {return s;}

	void setRed( double red) {r = red;}
	void setGreen( double green) {g = green;}
	void setBlue( double blue) {b = blue;}
	void setSpec( double spec) {s = spec;}

	double brightness(){
		return (r + g + b)/3.0;
	}

	Color colorScalar(double scalar){
		return Color(r*scalar, g*scalar, b*scalar, s);
	}

	Color colorAdd(Color C){
		return Color(r + C.getRed(), g + C.getGreen(), b + C.getBlue(), s);
	}

	Color colorMult(Color C){
		return Color(r*C.getRed(), g*C.getGreen(), b*C.getBlue(), s);
	}
	Color colorAverage(Color C){
		return Color((r+C.getRed())/2.0, (g+C.getGreen())/2.0, (b+C.getBlue())/2.0, s);
	}

	Color clip() {
		double allLight = r + g + b;
		double excessLight = allLight - 3;
		
		if(excessLight > 0){
			r = r + excessLight*(r/allLight);
			g = g + excessLight*(g/allLight);
			b = b + excessLight*(b/allLight);
		}
		
		if(r > 1){ r = 1;}
		if(g > 1){ g = 1;}
		if(b > 1){ b = 1;}
		if(r < 0){ r = 0;}
		if(g < 0){ g = 0;}
		if(b < 0){ b = 0;}
		
		return Color(r,g,b,s);
	}
	
};

Color::Color() 
{
	r = 0.5;
	g = 0.5;
	b = 0.5;
	s = 0;
}

Color::Color (double red, double green, double blue, double specularity)
{
	r = red;
	g = green;
	b = blue;
	s = specularity;
}
#endif
