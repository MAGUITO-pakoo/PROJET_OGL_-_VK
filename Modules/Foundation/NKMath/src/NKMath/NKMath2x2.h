#include <iostream>
#include <cmath>

struct NkVec2f {
    float x;
    float y;
    NkVec2f(float x = 0.0f, float y = 0.0f) {
        this->x = x;
        this->y = y;
    }
    void print() {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

struct Mat2x2 {
    float data[2][2];

    // Constructeur
    Mat2x2(float a, float b, float c, float d) {
        data[0][0] = a; 
        data[0][1] = b;
        data[1][0] = c; 
        data[1][1] = d;
    }

    // Afficheur de la matrice
    void print() {
        std::cout << "| " << data[0][0] << "  " << data[0][1] << " |\n";
        std::cout << "| " << data[1][0] << "  " << data[1][1] << " |\n";
    }

    // Addition
    Mat2x2 add(Mat2x2 autre) {
        return Mat2x2(
            data[0][0] + autre.data[0][0],  data[0][1] + autre.data[0][1],
            data[1][0] + autre.data[1][0],  data[1][1] + autre.data[1][1]
        );
    }

    // Transposée : échanger les lignes et les colonnes
    Mat2x2 transpose() {
        return Mat2x2(
            data[0][0], data[1][0],
            data[0][1], data[1][1]
        );
    }

    // Déterminant
    float determinant() {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }

    // Multiplication par un vecteur
    NkVec2f mulVec(NkVec2f v) {
        return NkVec2f(
            data[0][0] * v.x + data[0][1] * v.y,
            data[1][0] * v.x + data[1][1] * v.y
        );
    }

    // Multiplication par une auttre  matrice 
    Mat2x2 mulMat(Mat2x2 autre) {
        return Mat2x2(
            data[0][0]*autre.data[0][0] + data[0][1]*autre.data[1][0],
            data[0][0]*autre.data[0][1] + data[0][1]*autre.data[1][1],
            data[1][0]*autre.data[0][0] + data[1][1]*autre.data[1][0],
            data[1][0]*autre.data[0][1] + data[1][1]*autre.data[1][1]
        );
    }
};