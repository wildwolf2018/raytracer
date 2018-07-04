#ifndef PLANE_H
#define PLANE_H
#include "Vec3.h"
#include "Object.h"

class Plane : public Object
{
    public:
        Vec3 normal;
        float dist;

        Plane()
        {
            normal = Vec3(0.0, 1.0, 0.0);
            dist = 0;
            color = Vec3(1.0, 1.0 , 1.0);
            specular = 0;
            refractive = 0;
        }
        Plane(Vec3 normal, float dist, Vec3 color, bool _reflective)
        {
            this ->normal = normal;
             this -> dist = dist;
             this -> color = color;
        }
        bool  intersection(Vec3 rayOrigin, Vec3 rayDir, float &t0, float &t1)
        {
            float denom = rayDir.dot(normal);
            if(denom == 0)return false;
            float num = dist - (rayOrigin.dot(normal));
            t0 = (float)num / denom;
            return true;
        }
        Vec3 getNormal(Vec3 hitPoint)
        {
            return normal;
        }
        Vec3 getColor(){ return color;}
};
#endif
