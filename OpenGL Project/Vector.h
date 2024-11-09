#pragma once
#include<string>
#ifndef VECTOR_H
#define VECTOR_H

struct Vec3
{
public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) { }

    Vec3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) { }

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
};

//Syntax: vector * scalar
Vec3 operator*(const Vec3& vector, const float& scalar);

float DotProduct(const Vec3& a, const Vec3& b);
#endif