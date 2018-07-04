#ifndef VEC3_H
#define VEC3_H
#include <cmath>

class Vec3
{

   public:
    float x, y, z;
    Vec3() : x(float(0)), y(float(0)), z(float(0)) {}
    Vec3(float xx) : x(xx), y(xx), z(xx) {}
    Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vec3& normalize()
    {
        float temp = length2();
        if (temp > 0) {
            float num = 1 / sqrt(temp);
            x *= num, y *= num, z *= num;
        }
        return *this;
    }
    Vec3  operator * (const float &f) const
    {
        return Vec3(x * f, y * f, z * f);
    }
    Vec3 operator * (const Vec3 &v) const
    {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    float dot(const Vec3 &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vec3 operator - (const Vec3 &v) const
    {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator + (const Vec3 &v) const
    {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator += (const Vec3 &v)
    {
        x += v.x, y += v.y, z += v.z;
        return *this;
    }
    Vec3& operator *= (const Vec3 &v)
    {
        x *= v.x, y *= v.y, z *= v.z;
        return *this;
    }
    Vec3 operator - () const
    {
        return Vec3(-x, -y, -z);
    }
    float length2() const
    {
        return x * x + y * y + z * z;
    }
    float length() const
    {
        return sqrt(length2());
    }
};
#endif
