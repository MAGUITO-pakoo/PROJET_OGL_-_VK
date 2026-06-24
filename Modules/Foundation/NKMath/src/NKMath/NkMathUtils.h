#pragma once
#include <cmath>

// Fonctions et constantes mathématiques scalaires pour le moteur OpenGL.
// Tout est inline — aucun .cpp requis.
namespace NkMathUtils {

    // -------------------------------------------------------------------------
    // Constantes
    // -------------------------------------------------------------------------
    constexpr float PI         = 3.14159265358979323846f;
    constexpr float TWO_PI     = 6.28318530717958647692f;
    constexpr float HALF_PI    = 1.57079632679489661923f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;
    constexpr float EPSILON    = 1e-6f;

    // -------------------------------------------------------------------------
    // Conversion d'angles
    // -------------------------------------------------------------------------
    inline float deg2rad(float degrees) { return degrees * DEG_TO_RAD; }
    inline float rad2deg(float radians) { return radians * RAD_TO_DEG; }

    // -------------------------------------------------------------------------
    // Utilitaires scalaires
    // -------------------------------------------------------------------------
    template<typename T>
    inline T clamp(T v, T lo, T hi) {
        return v < lo ? lo : (v > hi ? hi : v);
    }

    template<typename T>
    inline T lerp(T a, T b, float t) {
        return a + (b - a) * t;
    }

    // Hermite smooth interpolation : 3t²-2t³  (t clamped en [0,1])
    inline float smoothstep(float edge0, float edge1, float x) {
        float t = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
        return t * t * (3.f - 2.f * t);
    }

    inline float sign(float v)  { return v > 0.f ? 1.f : (v < 0.f ? -1.f : 0.f); }
    inline float fract(float v) { return v - floorf(v); }

    inline float saturate(float v) { return clamp(v, 0.f, 1.f); }

} // namespace NkMathUtils
