#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>

struct NkVec2f {
    float x;
    float y;

    // Constructeur
    NkVec2f(float x = 0.0f, float y = 0.0f) {
        this->x = x;
        this->y = y;
    }
    NkVec2f operator+(const NkVec2f& o) const { return NkVec2f(x + o.x, y + o.y); }
    NkVec2f operator-(const NkVec2f& o) const { return NkVec2f(x - o.x, y - o.y); }
    NkVec2f operator*(float s)          const { return NkVec2f(x * s,   y * s); }

    // Addition
    NkVec2f add(NkVec2f autre) {
        return NkVec2f(x + autre.x, y + autre.y);
    }

    // Soustraction
    NkVec2f sub(NkVec2f autre) {
        return NkVec2f(x - autre.x, y - autre.y);
    }

    // multiplication par un scalaire
    NkVec2f scale(float s) {
        return NkVec2f(x * s, y * s);
    }

    // produit scalaire 
    float dot(NkVec2f autre) {
        return (x * autre.x) + (y * autre.y);
    }

    // longueur du vecteur
    float length() {
        return sqrt(x * x + y * y);
    }

    // normalisation
    NkVec2f normalize() {
        float l = length();
        return NkVec2f(x / l, y / l);
    }

   
    void print() {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};