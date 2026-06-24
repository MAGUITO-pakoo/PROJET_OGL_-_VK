#pragma once
#include <cmath>
#include <cstdio>

struct NkVec3f {
    float x, y, z;

    NkVec3f(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}

    // -------------------------------------------------------------------------
    // Opérateurs arithmétiques
    // -------------------------------------------------------------------------
    NkVec3f operator+(const NkVec3f& o) const { return { x+o.x, y+o.y, z+o.z }; }
    NkVec3f operator-(const NkVec3f& o) const { return { x-o.x, y-o.y, z-o.z }; }
    NkVec3f operator*(float s)          const { return { x*s,   y*s,   z*s   }; }
    NkVec3f operator/(float s)          const { return { x/s,   y/s,   z/s   }; }
    NkVec3f operator-()                 const { return { -x,    -y,    -z    }; }

    NkVec3f& operator+=(const NkVec3f& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    NkVec3f& operator-=(const NkVec3f& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
    NkVec3f& operator*=(float s)          { x*=s;   y*=s;   z*=s;   return *this; }
    NkVec3f& operator/=(float s)          { x/=s;   y/=s;   z/=s;   return *this; }

    bool operator==(const NkVec3f& o) const { return x==o.x && y==o.y && z==o.z; }
    bool operator!=(const NkVec3f& o) const { return !(*this == o); }

    // -------------------------------------------------------------------------
    // Opérations mathématiques
    // -------------------------------------------------------------------------
    float dot(const NkVec3f& o) const { return x*o.x + y*o.y + z*o.z; }

    NkVec3f cross(const NkVec3f& o) const {
        return { y*o.z - z*o.y,
                 z*o.x - x*o.z,
                 x*o.y - y*o.x };
    }

    float lengthSq() const { return x*x + y*y + z*z; }
    float length()   const { return sqrtf(lengthSq()); }

    NkVec3f normalize() const {
        float l = length();
        if (l < 1e-8f) return { 0.f, 0.f, 0.f };
        return { x/l, y/l, z/l };
    }

    // reflect(n) : réflexion de ce vecteur par rapport à la normale n (normalisée)
    // résultat = v - 2*(v·n)*n
    NkVec3f reflect(const NkVec3f& n) const {
        return *this - n * (2.f * dot(n));
    }

    // -------------------------------------------------------------------------
    // Noms legacy (compatibilité avec anciens exercices)
    // -------------------------------------------------------------------------
    NkVec3f add(const NkVec3f& o)  const { return *this + o; }
    NkVec3f sub(const NkVec3f& o)  const { return *this - o; }
    NkVec3f scale(float s)          const { return *this * s; }

    void print() const { printf("(%.4f, %.4f, %.4f)\n", x, y, z); }
};

// Permet s * vec en plus de vec * s
inline NkVec3f operator*(float s, const NkVec3f& v) { return v * s; }
