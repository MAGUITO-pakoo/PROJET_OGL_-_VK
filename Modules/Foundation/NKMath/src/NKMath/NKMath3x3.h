#include <iostream>
#include <cmath>


struct NkVec3f {
    float x, y, z;

    NkVec3f(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void print() {
        std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }
};

struct Mat3x3 {
    float data[3][3];

    // Constructeur
    Mat3x3(float a, float b, float c,
           float d, float e, float f,
           float g, float h, float i) {
        data[0][0]=a; data[0][1]=b; data[0][2]=c;
        data[1][0]=d; data[1][1]=e; data[1][2]=f;
        data[2][0]=g; data[2][1]=h; data[2][2]=i;
    }

    // Afficheur 
    void print() {

        std::cout << "| " << data[0][0] << "  " << data[0][1] <<  "  " << data[0][2] << "|\n";
        std::cout << "| " << data[1][0] << "  " << data[1][1] <<  "  " << data[1][2] << "|\n";
        std::cout << "| " << data[2][0] << "  " << data[2][1] <<  "  " << data[2][2] << "|\n";

       /*  
        for (int i = 0; i < 3; i++) {
            std::cout << "| ";
            for (int j = 0; j < 3; j++)
                std::cout << data[i][j] << " ";
            std::cout << "|\n";
        } */
    }

    // Addition
    Mat3x3 add(Mat3x3 autre) {
        return Mat3x3(
            data[0][0]+autre.data[0][0], data[0][1]+autre.data[0][1], data[0][2]+autre.data[0][2],
            data[1][0]+autre.data[1][0], data[1][1]+autre.data[1][1], data[1][2]+autre.data[1][2],
            data[2][0]+autre.data[2][0], data[2][1]+autre.data[2][1], data[2][2]+autre.data[2][2]
        );
    }

    // Transposée
    Mat3x3 transpose() {
        return Mat3x3(
            data[0][0], data[1][0], data[2][0],
            data[0][1], data[1][1], data[2][1],
            data[0][2], data[1][2], data[2][2]
        );
    }

    // Déterminant (règle de Sarrus)
    float determinant() {
        return data[0][0] * (data[1][1]*data[2][2] - data[1][2]*data[2][1])
             - data[0][1] * (data[1][0]*data[2][2] - data[1][2]*data[2][0])
             + data[0][2] * (data[1][0]*data[2][1] - data[1][1]*data[2][0]);
    }

    // Multiplication par un vecteur
    NkVec3f mulVec(NkVec3f v) {
        return NkVec3f(
            data[0][0]*v.x + data[0][1]*v.y + data[0][2]*v.z,
            data[1][0]*v.x + data[1][1]*v.y + data[1][2]*v.z,
            data[2][0]*v.x + data[2][1]*v.y + data[2][2]*v.z
        );
    }

    // Multiplication par  matrice
    Mat3x3 mulMat(Mat3x3 o) {
        return Mat3x3(
            data[0][0]*o.data[0][0] + data[0][1]*o.data[1][0] + data[0][2]*o.data[2][0],
            data[0][0]*o.data[0][1] + data[0][1]*o.data[1][1] + data[0][2]*o.data[2][1],
            data[0][0]*o.data[0][2] + data[0][1]*o.data[1][2] + data[0][2]*o.data[2][2],

            data[1][0]*o.data[0][0] + data[1][1]*o.data[1][0] + data[1][2]*o.data[2][0],
            data[1][0]*o.data[0][1] + data[1][1]*o.data[1][1] + data[1][2]*o.data[2][1],
            data[1][0]*o.data[0][2] + data[1][1]*o.data[1][2] + data[1][2]*o.data[2][2],

            data[2][0]*o.data[0][0] + data[2][1]*o.data[1][0] + data[2][2]*o.data[2][0],
            data[2][0]*o.data[0][1] + data[2][1]*o.data[1][1] + data[2][2]*o.data[2][1],
            data[2][0]*o.data[0][2] + data[2][1]*o.data[1][2] + data[2][2]*o.data[2][2]
        );
    }
};