#pragma once
#include "NKVec3f.h"
#include <cmath>
#include <cstdio>

struct NkVec4f {
    float x, y, z, w;

    NkVec4f(float x=0.f, float y=0.f, float z=0.f, float w=0.f)
        : x(x), y(y), z(z), w(w) {}

    // Construction depuis vec3 (utile pour gl_Position = vec4(pos, 1.0))
    NkVec4f(const NkVec3f& v, float w = 1.f) : x(v.x), y(v.y), z(v.z), w(w) {}

    // -------------------------------------------------------------------------
    // Swizzle
    // -------------------------------------------------------------------------
    NkVec3f xyz() const { return { x, y, z }; }

    // -------------------------------------------------------------------------
    // Opérateurs arithmétiques
    // -------------------------------------------------------------------------
    NkVec4f operator+(const NkVec4f& o) const { return {x+o.x, y+o.y, z+o.z, w+o.w}; }
    NkVec4f operator-(const NkVec4f& o) const { return {x-o.x, y-o.y, z-o.z, w-o.w}; }
    NkVec4f operator*(float s)          const { return {x*s,   y*s,   z*s,   w*s  }; }
    NkVec4f operator/(float s)          const { return {x/s,   y/s,   z/s,   w/s  }; }
    NkVec4f operator-()                 const { return {-x,    -y,    -z,    -w   }; }

    NkVec4f& operator+=(const NkVec4f& o) { x+=o.x; y+=o.y; z+=o.z; w+=o.w; return *this; }
    NkVec4f& operator-=(const NkVec4f& o) { x-=o.x; y-=o.y; z-=o.z; w-=o.w; return *this; }
    NkVec4f& operator*=(float s)          { x*=s;   y*=s;   z*=s;   w*=s;   return *this; }

    bool operator==(const NkVec4f& o) const { return x==o.x && y==o.y && z==o.z && w==o.w; }
    bool operator!=(const NkVec4f& o) const { return !(*this == o); }

    // -------------------------------------------------------------------------
    // Opérations mathématiques
    // -------------------------------------------------------------------------
    float dot(const NkVec4f& o) const { return x*o.x + y*o.y + z*o.z + w*o.w; }
    float lengthSq()             const { return x*x + y*y + z*z + w*w; }
    float length()               const { return sqrtf(lengthSq()); }

    NkVec4f normalize() const {
        float l = length();
        if (l < 1e-8f) return {0.f, 0.f, 0.f, 0.f};
        return {x/l, y/l, z/l, w/l};
    }

    // -------------------------------------------------------------------------
    // Noms legacy (compatibilité)
    // -------------------------------------------------------------------------
    NkVec4f add(NkVec4f o)  const { return *this + o; }
    NkVec4f sub(NkVec4f o)  const { return *this - o; }
    NkVec4f scale(float s)   const { return *this * s; }

    void print() const { printf("(%.4f, %.4f, %.4f, %.4f)\n", x, y, z, w); }
};

inline NkVec4f operator*(float s, const NkVec4f& v) { return v * s; }
