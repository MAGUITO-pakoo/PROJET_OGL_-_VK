#include <iostream>
#include <cstdint>

struct NkVec2i {
    int x;
    int y;

    // Constructeur
    NkVec2i(int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }
    // Addition
    NkVec2i add(NkVec2i autre) {
        return NkVec2i(x + autre.x, y + autre.y);
    }

    // Soustraction
    NkVec2i sub(NkVec2i autre) {
        return NkVec2i(x - autre.x, y - autre.y);
    }

    // Multiplication par un scalaire
    NkVec2i scale(int s) {
        return NkVec2i(x * s, y * s);
    }

    void print() {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

