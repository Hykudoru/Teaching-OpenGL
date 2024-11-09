#pragma once
#ifndef MATRIX_H
#define MATRIX_H
#include "Vector.h"
#include <math.h>

class Matrix3x3
{
public:
    float m[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    static float identity[3][3];
    static float zero[3][3];

    Matrix3x3() {}
    Matrix3x3(const float matrix3x3[3][3])
    {
        for (size_t r = 0; r < 3; r++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                this->m[r][c] = matrix3x3[r][c];
            }
        }
    }

    static Matrix3x3 RotX(float theta)
    {
        float Cos = cos(theta);
        float Sin = sin(theta);

        float standardRotX[3][3] =
        {
            {1, 0, 0},
            {0, Cos, -Sin},
            {0, Sin, Cos}
        };

        Matrix3x3 matrix(standardRotX);
        return matrix;
    }

    static Matrix3x3 RotY(float theta)
    {
        float Cos = cos(theta);
        float Sin = sin(theta);

        float standardRotY[3][3] =
        {
            {Cos, 0, Sin},
            {0,   1,  0 },
            {-Sin,0, Cos}
        };

        Matrix3x3 matrix(standardRotY);
        return matrix;
    }

    static Matrix3x3 RotZ(float theta)
    {
        float Cos = cos(theta);
        float Sin = sin(theta);

        float standardRotZ[3][3] =
        {
            {Cos, -Sin,  0},
            {Sin,  Cos,  0},
            {0,     0,   1}
        };

        Matrix3x3 matrix(standardRotZ);
        return matrix;
    }
};

// A * B
Matrix3x3 operator*(const Matrix3x3& matrixA, const Matrix3x3& matrixB);

// M * p
Vec3 operator*(const Matrix3x3& matrix, const Vec3& colVec);

#endif