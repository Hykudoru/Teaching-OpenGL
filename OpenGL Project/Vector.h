#pragma once
#include<string>
#ifndef VECTOR_H
#define VECTOR_H

struct Vec4;

struct Vec3
{
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) {}

    Vec3 operator+(const Vec3& other)
    {
        Vec3 vectorSum(this->x + other.x, this->y + other.y, this->z + other.z);
        return vectorSum;
    }

    Vec3 operator-(const Vec3& other)
    {
        Vec3 vectorDiff(this->x - other.x, this->y - other.y, this->z - other.z);
        return vectorDiff;
    }

    Vec3 operator*(const float& scalar)
    {
        Vec3 scaledVector(this->x * scalar, this->y * scalar, this->z * scalar);
        return scaledVector;
    }

    Vec3& operator+=(const Vec3& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    Vec3& operator*=(const float scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }

    operator Vec4();
};

float DotProduct(const Vec3& a, const Vec3& b);


struct Vec4
{
public:
    float x, y, z, w;

    Vec4() : x(0), y(0), z(0), w(1.0f) {}

    Vec4(float xVal, float yVal, float zVal, float wVal) : x(xVal), y(yVal), z(zVal), w(wVal) {}

    operator Vec3();
};

float DotProduct(const Vec4& a, const Vec4& b);
#endif