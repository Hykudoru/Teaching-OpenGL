#include "Matrix.h"

float Matrix3x3::identity[3][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};

// A * B
Matrix3x3 operator*(const Matrix3x3& matrixA, const Matrix3x3& matrixB)
{
    Matrix3x3 result = Matrix3x3();
    for (size_t r = 0; r < 3; r++)
    {
        Vec3 rowVec = Vec3(matrixA.m[r][0], matrixA.m[r][1], matrixA.m[r][2]);
        for (size_t c = 0; c < 3; c++)
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
Vec3 operator*(const Matrix3x3& matrix, const Vec3& colVec)
{
    Vec3 result = Vec3();
    Vec3 row1 = Vec3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2]);
    Vec3 row2 = Vec3(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2]);
    Vec3 row3 = Vec3(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2]);
    result.x = DotProduct(row1, colVec);
    result.y = DotProduct(row2, colVec);
    result.z = DotProduct(row3, colVec);

    return result;
}