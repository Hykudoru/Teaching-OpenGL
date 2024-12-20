#include "Matrix.h"

float Matrix3x3::identity[3][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};

float Matrix4x4::identity[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

// A * B
Matrix4x4 operator*(const Matrix4x4& matrixA, const Matrix4x4& matrixB)
{
    Matrix4x4 result = Matrix4x4();
    for (size_t r = 0; r < 4; r++)
    {
        Vec4 rowVec = Vec4(matrixA.m[r][0], matrixA.m[r][1], matrixA.m[r][2], matrixA.m[r][3]);
        for (size_t c = 0; c < 4; c++)
        {
            Vec4 columnVec;
            columnVec.x = matrixB.m[0][c];
            columnVec.y = matrixB.m[1][c];
            columnVec.z = matrixB.m[2][c];
            columnVec.w = matrixB.m[3][c];
            result.m[r][c] = DotProduct(rowVec, columnVec);
        }
    }

    return result;
}

// M * p
Vec4 operator*(const Matrix4x4& matrix, const Vec4& colVec)
{
    Vec4 result = Vec4();
    Vec4 row1 = Vec4(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2], matrix.m[0][3]);
    Vec4 row2 = Vec4(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2], matrix.m[1][3]);
    Vec4 row3 = Vec4(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2], matrix.m[2][3]);
    Vec4 row4 = Vec4(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]);
    result.x = DotProduct(row1, colVec);
    result.y = DotProduct(row2, colVec);
    result.z = DotProduct(row3, colVec);
    result.w = DotProduct(row4, colVec);

    return result;
}