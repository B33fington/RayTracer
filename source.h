#ifndef _SOURCE_H
#define _SOURCE_H

#include "precompiled.h"

class Source {
	public:

		Source();

		virtual vec3 getLightPosition() {return vec3(0,0,0);};
		virtual Color getLightColor() {return Color(1,1,1,0);};

};

Source::Source() {}

#endif