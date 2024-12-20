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

    // A * B
    Matrix3x3 operator*(const Matrix3x3& matrixB)
    {
        Matrix3x3 result = Matrix3x3();
        for (size_t r = 0; r < 3; r++)
        {
            Vec3 rowVec = Vec3(this->m[r][0], this->m[r][1], this->m[r][2]);
            for (int c = 0; c < 3; c++)
            {
                Vec3 columnVec;
                columnVec.x = matrixB.m[0][c];
                columnVec.y = matrixB.m[1][c];
                columnVec.z = matrixB.m[2][c];
                result.m[r][c] = DotProduct(rowVec, columnVec);
            }
        }

        return result;
    }

    // M * p
    Vec3 operator*(const Vec3& colVec)
    {
        Vec3 result = Vec3();
        Vec3 row1 = Vec3(this->m[0][0], this->m[0][1], this->m[0][2]);
        Vec3 row2 = Vec3(this->m[1][0], this->m[1][1], this->m[1][2]);
        Vec3 row3 = Vec3(this->m[2][0], this->m[2][1], this->m[2][2]);
        result.x = DotProduct(row1, colVec);
        result.y = DotProduct(row2, colVec);
        result.z = DotProduct(row3, colVec);

        return result;
    }
};

class Matrix4x4
{
public:
    float m[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    static float identity[4][4];
    static float zero[4][4];

    Matrix4x4() {}
    Matrix4x4(const float matrix[4][4])
    {
        for (size_t r = 0; r < 4; r++)
        {
            for (size_t c = 0; c < 4; c++)
            {
                this->m[r][c] = matrix[r][c];
            }
        }
    }
};

// A * B
Matrix4x4 operator*(const Matrix4x4& matrixA, const Matrix4x4& matrixB);

// M * p
Vec4 operator*(const Matrix4x4& matrix, const Vec4& colVec);
#endif