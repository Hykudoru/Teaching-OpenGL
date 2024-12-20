#include "Vector.h"

float DotProduct(const Vec3& a, const Vec3& b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vec3::operator Vec4()
{
    Vec4 vec4(x, y, z, 1);
    return vec4;
}

Vec4::operator Vec3()
{
    Vec3 vec3(x, y, z);
    return vec3;
}

float DotProduct(const Vec4& a, const Vec4& b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}