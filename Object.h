#ifndef OBJECT_H
#define OBJECT_H
#include "Vec3.h"

class Object
{

	public:
	    float specular;
	    float refractive;
	    Vec3 color;
        virtual Vec3 getColor() = 0;
		virtual bool intersection(Vec3 origin, Vec3 dir, float &t0, float &t1) = 0;
		virtual Vec3 getNormal(Vec3 hitPoint) = 0;

};
#endif
