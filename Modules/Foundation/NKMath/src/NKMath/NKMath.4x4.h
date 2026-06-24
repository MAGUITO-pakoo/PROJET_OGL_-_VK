#include <iostream>
#include <cmath>

struct NkVec4f {
    float x, y, z, w;

    NkVec4f(float x=0.0f, float y=0.0f, float z=0.0f, float w=0.0f) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void print() {
        std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")\n";
    }
};

struct Mat4x4 {
    float data[4][4];

    // Constructeur
    Mat4x4(float a0, float a1, float a2, float a3,
           float b0, float b1, float b2, float b3,
           float c0, float c1, float c2, float c3,
           float d0, float d1, float d2, float d3) {
        data[0][0]=a0; data[0][1]=a1; data[0][2]=a2; data[0][3]=a3;
        data[1][0]=b0; data[1][1]=b1; data[1][2]=b2; data[1][3]=b3;
        data[2][0]=c0; data[2][1]=c1; data[2][2]=c2; data[2][3]=c3;
        data[3][0]=d0; data[3][1]=d1; data[3][2]=d2; data[3][3]=d3;
    }

    // Matrice identité : 1 sur la diagonale, 0 ailleurs
    static Mat4x4 identity() {
        return Mat4x4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    // Afficher
    void print() {

        std::cout << "| " << data[0][0] << "  " << data[0][1] << "  " << data[0][2] << "  " << data[0][3] << "|\n";
        std::cout << "| " << data[1][0] << "  " << data[1][1] << "  " << data[1][2] << "  " << data[1][3] << "|\n";
        std::cout << "| " << data[2][0] << "  " << data[2][1] << "  " << data[2][2] << "  " << data[2][3] << "|\n";
        std::cout << "| " << data[3][0] << "  " << data[3][1] << "  " << data[3][2] << "  " << data[3][3] << "|\n";

       /*  for (int i = 0; i < 4; i++) {
            std::cout << "| ";
            for (int j = 0; j < 4; j++)
                std::cout << data[i][j] << " ";
            std::cout << "|\n";
        } */
    }

    // Transposée
    Mat4x4 transpose() {
        return Mat4x4(
            data[0][0], data[1][0], data[2][0], data[3][0],
            data[0][1], data[1][1], data[2][1], data[3][1],
            data[0][2], data[1][2], data[2][2], data[3][2],
            data[0][3], data[1][3], data[2][3], data[3][3]
        );
    }

    // Addition
    Mat4x4 add(Mat4x4 o) {
        return Mat4x4(
            data[0][0]+o.data[0][0], data[0][1]+o.data[0][1], data[0][2]+o.data[0][2], data[0][3]+o.data[0][3],
            data[1][0]+o.data[1][0], data[1][1]+o.data[1][1], data[1][2]+o.data[1][2], data[1][3]+o.data[1][3],
            data[2][0]+o.data[2][0], data[2][1]+o.data[2][1], data[2][2]+o.data[2][2], data[2][3]+o.data[2][3],
            data[3][0]+o.data[3][0], data[3][1]+o.data[3][1], data[3][2]+o.data[3][2], data[3][3]+o.data[3][3]
        );
    }

    // Multiplication matrice × vecteur
    NkVec4f mulVec(NkVec4f v) {
        return NkVec4f(
            data[0][0]*v.x + data[0][1]*v.y + data[0][2]*v.z + data[0][3]*v.w,
            data[1][0]*v.x + data[1][1]*v.y + data[1][2]*v.z + data[1][3]*v.w,
            data[2][0]*v.x + data[2][1]*v.y + data[2][2]*v.z + data[2][3]*v.w,
            data[3][0]*v.x + data[3][1]*v.y + data[3][2]*v.z + data[3][3]*v.w
        );
    }

    // Multiplication matrice × matrice
    Mat4x4 mulMat(Mat4x4 o) {
        Mat4x4 r = identity();
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                r.data[i][j] = 0;
                for (int k = 0; k < 4; k++)
                    r.data[i][j] += data[i][k] * o.data[k][j];
            }
        return r;
    }
};