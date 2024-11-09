#include "Vector.h"

//Syntax: vector * scalar
Vec3 operator*(const Vec3& vector, const float& scalar)
{
    Vec3 scaledVector = vector;
    scaledVector.x = scaledVector.x * scalar;
    scaledVector.y = scaledVector.y * scalar;
    scaledVector.z = scaledVector.z * scalar;
    return scaledVector;
}

float DotProduct(const Vec3& a, const Vec3& b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}