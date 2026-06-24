#include <iostream>
#include <cstdint>
#include <NKMath/NKVec2i.h>
#include <NKMath/NKVec2f.h>
#include <NKMath/NKVec3f.h>
#include <NKMath/NKVec4f.h>
#include <NKMath/NKMath2x2.h>
#include <NKMath/NKMath3x3.h>
#include <NKMath/NKMath.4x4.h>

int main() {
    std::cout << "hello world\n\n";
    std::cout << "-------vecteurs entier 2D-------" << std::endl; 
    NkVec2i v1(3, 4);
    NkVec2i v2(1, 2);

    std::cout << "v1 = "; v1.print();
    std::cout << "v2 = "; v2.print();

    NkVec2i somme = v1.add(v2);
    std::cout << "v1 +v2 = "; 
    somme.print();

    NkVec2i diff = v1.sub(v2);
    std::cout << "v1 - v2 = "; 
    diff.print();

    NkVec2i scale = v1.scale(3);
    std::cout << "v1 * 3 = "; 
    scale.print();

    std::cout << "-------vecteurs entier 3D-------" << std::endl;



    return 0;
}