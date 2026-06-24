#pragma once
#include "NKVec3f.h"
#include "NkMat4x4.h"
#include <cmath>
#include <cstdio>

// Quaternion d'unité pour les rotations 3D : q = xi + yj + zk + w
// Convention : w = partie réelle, (x,y,z) = partie imaginaire
// Un quaternion unitaire (|q|=1) représente une rotation sans singularité

struct NkQuaternion {
    float x, y, z, w;

    // Constructeur par défaut : quaternion identité (aucune rotation)
    NkQuaternion(float x=0.f, float y=0.f, float z=0.f, float w=1.f)
        : x(x), y(y), z(z), w(w) {}

    // -------------------------------------------------------------------------
    // Constructeurs statiques
    // -------------------------------------------------------------------------

    // Rotation d'un angle (radians) autour d'un axe normalisé
    static NkQuaternion fromAxisAngle(const NkVec3f& axis, float angle) {
        float half = angle * 0.5f;
        float s    = sinf(half);
        return { axis.x*s, axis.y*s, axis.z*s, cosf(half) };
    }

    // Identité (aucune rotation)
    static NkQuaternion identity() {
        return { 0.f, 0.f, 0.f, 1.f };
    }

    // -------------------------------------------------------------------------
    // Opérations
    // -------------------------------------------------------------------------

    // Composition de rotations : q * p applique p d'abord, puis q
    NkQuaternion operator*(const NkQuaternion& o) const {
        return {
            w*o.x + x*o.w + y*o.z - z*o.y,
            w*o.y - x*o.z + y*o.w + z*o.x,
            w*o.z + x*o.y - y*o.x + z*o.w,
            w*o.w - x*o.x - y*o.y - z*o.z
        };
    }

    // Conjugué : représente la rotation inverse (pour un quaternion unitaire)
    NkQuaternion conjugate()  const { return { -x, -y, -z, w }; }

    float lengthSq() const { return x*x + y*y + z*z + w*w; }
    float length()   const { return sqrtf(lengthSq()); }

    NkQuaternion normalize() const {
        float l = length();
        if (l < 1e-8f) return identity();
        return { x/l, y/l, z/l, w/l };
    }

    // Interpolation sphérique entre deux rotations
    // t=0 → retourne a, t=1 → retourne b
    static NkQuaternion slerp(const NkQuaternion& a, const NkQuaternion& b, float t) {
        float dot = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
        // Choisir l'arc le plus court (dot < 0 → flip b)
        NkQuaternion bb = b;
        if (dot < 0.f) {
            bb = { -b.x, -b.y, -b.z, -b.w };
            dot = -dot;
        }
        // Proche de parallèle : interpolation linéaire normalisée (évite division par 0)
        if (dot > 0.9995f) {
            NkQuaternion r = {
                a.x + t*(bb.x - a.x),
                a.y + t*(bb.y - a.y),
                a.z + t*(bb.z - a.z),
                a.w + t*(bb.w - a.w)
            };
            return r.normalize();
        }
        float theta0    = acosf(dot);          // angle entre a et b
        float theta     = theta0 * t;          // angle d'interpolation
        float sinTheta  = sinf(theta);
        float sinTheta0 = sinf(theta0);
        float s0 = cosf(theta) - dot * sinTheta / sinTheta0;
        float s1 = sinTheta / sinTheta0;
        return {
            s0*a.x + s1*bb.x,
            s0*a.y + s1*bb.y,
            s0*a.z + s1*bb.z,
            s0*a.w + s1*bb.w
        };
    }

    // Conversion vers matrice de rotation 4x4 (column-major, compatible NkMat4x4)
    // Le quaternion doit être unitaire (normalize() avant si nécessaire)
    NkMat4x4 toMat4() const {
        NkQuaternion q = normalize();
        float xx=q.x*q.x, yy=q.y*q.y, zz=q.z*q.z;
        float xy=q.x*q.y, xz=q.x*q.z, yz=q.y*q.z;
        float wx=q.w*q.x, wy=q.w*q.y, wz=q.w*q.z;

        NkMat4x4 mat = NkMat4x4::identity();
        // Colonne 0 : vecteur 'right' transformé
        mat.m[0][0] = 1.f - 2.f*(yy+zz);
        mat.m[0][1] = 2.f*(xy+wz);
        mat.m[0][2] = 2.f*(xz-wy);
        // Colonne 1 : vecteur 'up' transformé
        mat.m[1][0] = 2.f*(xy-wz);
        mat.m[1][1] = 1.f - 2.f*(xx+zz);
        mat.m[1][2] = 2.f*(yz+wx);
        // Colonne 2 : vecteur 'forward' transformé
        mat.m[2][0] = 2.f*(xz+wy);
        mat.m[2][1] = 2.f*(yz-wx);
        mat.m[2][2] = 1.f - 2.f*(xx+yy);
        return mat;
    }

    void print() const {
        printf("Quat(x=%.4f, y=%.4f, z=%.4f, w=%.4f)\n", x, y, z, w);
    }
};
