#pragma once
#include "NKVec3f.h"
#include "NkMat4x4.h"
#include "NkQuaternion.h"
#include <cstdio>

// Représente la transformation complète d'un objet dans la scène :
//   position (translation), rotation (quaternion), scale (mise à l'échelle)
//
// toMat4() produit la matrice TRS = T * R * S
// L'ordre est crucial : scale d'abord, rotation ensuite, translation en dernier.
// C'est l'ordre standard dans les moteurs de jeu (Unity, Unreal, etc.)

struct NkTransform3D {
    NkVec3f     position;  // translation dans le monde
    NkQuaternion rotation; // orientation (quaternion unitaire)
    NkVec3f     scaleVec;  // échelle non-uniforme possible

    // Constructeur par défaut : objet à l'origine, sans rotation, échelle 1
    NkTransform3D()
        : position (0.f, 0.f, 0.f)
        , rotation (NkQuaternion::identity())
        , scaleVec (1.f, 1.f, 1.f)
    {}

    NkTransform3D(const NkVec3f& pos,
                  const NkQuaternion& rot,
                  const NkVec3f& sc)
        : position(pos), rotation(rot), scaleVec(sc)
    {}

    // Helpers de construction courants
    static NkTransform3D atPosition(const NkVec3f& pos) {
        NkTransform3D t;
        t.position = pos;
        return t;
    }

    static NkTransform3D withScale(const NkVec3f& sc) {
        NkTransform3D t;
        t.scaleVec = sc;
        return t;
    }

    // -------------------------------------------------------------------------
    // Conversion en matrice modèle (Model Matrix) : TRS
    // -------------------------------------------------------------------------
    NkMat4x4 toMat4() const {
        NkMat4x4 T = NkMat4x4::translation(position);
        NkMat4x4 R = rotation.toMat4();
        NkMat4x4 S = NkMat4x4::scale(scaleVec);
        return T * R * S;
    }

    void print() const {
        printf("NkTransform3D {\n");
        printf("  position : (%.4f, %.4f, %.4f)\n",
               position.x, position.y, position.z);
        printf("  rotation : ");
        rotation.print();
        printf("  scale    : (%.4f, %.4f, %.4f)\n",
               scaleVec.x, scaleVec.y, scaleVec.z);
        printf("}\n");
    }
};
