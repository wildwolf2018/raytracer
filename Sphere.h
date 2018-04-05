#ifndef SPHERE_H
#define SPHERE_H
#include <cmath>
#include "Vec3.h"
#include "Object.h"

class Sphere: public Object
{
   public:
        Vec3 center;
        float radius;

        Sphere()
        {
           center = Vec3(0.0, 0.0, -8);
           radius = 1;
           color = Vec3(0.0, 1.0, 0.0);
           specular = 0.0;
           refractive = 0.7;
        }

        Sphere(Vec3 center, float radius, Vec3 color, float _specular,float _refractive)
        {
            this->center = center;
            this->radius = radius;
            this->color = color;
            specular = _specular;
            refractive = _refractive;
        }

        bool intersection(Vec3 rayOrigin, Vec3 rayDir, float &t0, float &t1)
        {
            Vec3 b1 = rayDir * 2.0f;
            Vec3 b2 = rayOrigin - center;
            float b = b1.dot(b2);
            float c = b2.dot(b2) - (radius * radius);
            float disc = b * b - 4.0 * c;
            if(disc < 0)
            {
                return false;
            }
            if(disc > 0)
            {
                 t0 =  (float)(-1 * b - sqrt(disc))/ 2.0;
                 t1 =  (float)(-1 * b + sqrt(disc))/ 2.0;
                 if(t0 < 0 && t1 < 0)return false;
                 if(t0 <0 && t1 > 0) return true;
                 if(t0 >0 && t1 < 0)return true;
                 if(t0 >0 && t1 > 0)return true;
            }
            return false;
        }
        Vec3 getColor(){ return color;}
        Vec3 getNormal(Vec3 hitPoint){ return ( hitPoint - center);}
};
#endif
