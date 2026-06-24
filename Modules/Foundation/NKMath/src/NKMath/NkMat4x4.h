#pragma once
#include "NKVec3f.h"
#include "NKVec4f.h"
#include <cmath>
#include <cstdio>

// Matrice 4x4 column-major : m[col][row]
// Compatible directement avec glUniformMatrix4fv(..., GL_FALSE, mat.data())
// Ordre de stockage linéaire : col0row0, col0row1, col0row2, col0row3, col1row0, ...
//
// Rappel visuel : la matrice de translation T(tx,ty,tz) donne :
//   m[0]={1,0,0,0}  m[1]={0,1,0,0}  m[2]={0,0,1,0}  m[3]={tx,ty,tz,1}
//   affichée en colonnes → représentation mathématique habituelle (lignes) :
//   | 1  0  0  tx |
//   | 0  1  0  ty |
//   | 0  0  1  tz |
//   | 0  0  0   1 |

struct NkMat4x4 {
    float m[4][4]; // m[col][row]

    NkMat4x4() {
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                m[c][r] = 0.f;
    }

    // -------------------------------------------------------------------------
    // Constructeurs statiques
    // -------------------------------------------------------------------------

    static NkMat4x4 identity() {
        NkMat4x4 mat;
        mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.f;
        return mat;
    }

    static NkMat4x4 translation(const NkVec3f& t) {
        NkMat4x4 mat = identity();
        mat.m[3][0] = t.x; // colonne 3, ligne 0
        mat.m[3][1] = t.y;
        mat.m[3][2] = t.z;
        return mat;
    }

    static NkMat4x4 scale(const NkVec3f& s) {
        NkMat4x4 mat = identity();
        mat.m[0][0] = s.x;
        mat.m[1][1] = s.y;
        mat.m[2][2] = s.z;
        return mat;
    }

    // Rotation de Rodrigues — axis doit être normalisé, angle en radians
    static NkMat4x4 rotation(const NkVec3f& axis, float angle) {
        NkVec3f a = axis.normalize();
        float c = cosf(angle);
        float s = sinf(angle);
        float t = 1.f - c;
        float x = a.x, y = a.y, z = a.z;

        NkMat4x4 mat = identity();
        // Colonne 0
        mat.m[0][0] = t*x*x + c;
        mat.m[0][1] = t*x*y + s*z;
        mat.m[0][2] = t*x*z - s*y;
        // Colonne 1
        mat.m[1][0] = t*x*y - s*z;
        mat.m[1][1] = t*y*y + c;
        mat.m[1][2] = t*y*z + s*x;
        // Colonne 2
        mat.m[2][0] = t*x*z + s*y;
        mat.m[2][1] = t*y*z - s*x;
        mat.m[2][2] = t*z*z + c;
        return mat;
    }

    // Projection perspective (convention OpenGL right-handed, depth [-1,1])
    // fovY en radians, aspect = largeur/hauteur
    static NkMat4x4 perspective(float fovY, float aspect, float nearZ, float farZ) {
        float tanHalf = tanf(fovY * 0.5f);
        NkMat4x4 mat; // tout à zéro
        mat.m[0][0] =  1.f / (aspect * tanHalf);
        mat.m[1][1] =  1.f / tanHalf;
        mat.m[2][2] = -(farZ + nearZ) / (farZ - nearZ);
        mat.m[2][3] = -1.f;                                     // met w_clip = -z_eye
        mat.m[3][2] = -(2.f * farZ * nearZ) / (farZ - nearZ);
        // mat.m[3][3] = 0 (déjà initialisé)
        return mat;
    }

    // Vue caméra (convention OpenGL right-handed)
    // eye = position caméra, center = point regardé, up = axe haut du monde
    static NkMat4x4 lookAt(const NkVec3f& eye, const NkVec3f& center, const NkVec3f& up) {
        NkVec3f f = (center - eye).normalize(); // forward
        NkVec3f r = f.cross(up).normalize();    // right
        NkVec3f u = r.cross(f);                 // up réel (orthogonalisé)

        NkMat4x4 mat = identity();
        // Les 3 premières colonnes : axes de la caméra exprimés dans le monde
        mat.m[0][0] =  r.x;  mat.m[0][1] =  u.x;  mat.m[0][2] = -f.x;
        mat.m[1][0] =  r.y;  mat.m[1][1] =  u.y;  mat.m[1][2] = -f.y;
        mat.m[2][0] =  r.z;  mat.m[2][1] =  u.z;  mat.m[2][2] = -f.z;
        // Colonne 3 : translation (= -R * eye)
        mat.m[3][0] = -r.dot(eye);
        mat.m[3][1] = -u.dot(eye);
        mat.m[3][2] =  f.dot(eye);
        return mat;
    }

    // -------------------------------------------------------------------------
    // Opérateurs
    // -------------------------------------------------------------------------

    // Multiplication matrice × matrice  C = A * B
    // C.m[c][r] = Σk A.m[k][r] * B.m[c][k]
    NkMat4x4 operator*(const NkMat4x4& o) const {
        NkMat4x4 result;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                for (int k = 0; k < 4; k++)
                    result.m[c][r] += m[k][r] * o.m[c][k];
        return result;
    }

    // Multiplication matrice × vecteur (vecteur colonne)
    NkVec4f operator*(const NkVec4f& v) const {
        return {
            m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0]*v.w,
            m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1]*v.w,
            m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]*v.w,
            m[0][3]*v.x + m[1][3]*v.y + m[2][3]*v.z + m[3][3]*v.w
        };
    }

    // Transposée : échange lignes et colonnes
    NkMat4x4 transpose() const {
        NkMat4x4 result;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                result.m[r][c] = m[c][r];
        return result;
    }

    // Inverse par élimination de Gauss-Jordan avec pivot partiel
    // Retourne identity() si la matrice est singulière
    NkMat4x4 inverse() const {
        // Matrice augmentée [M | I] en row-major temporaire
        float aug[4][8];
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) aug[r][c] = m[c][r]; // column-major → row-major
            for (int c = 4; c < 8; c++) aug[r][c] = (c-4 == r) ? 1.f : 0.f;
        }
        for (int pivot = 0; pivot < 4; pivot++) {
            // Trouver la ligne avec le plus grand pivot (stabilité numérique)
            int maxRow = pivot;
            for (int r = pivot+1; r < 4; r++)
                if (fabsf(aug[r][pivot]) > fabsf(aug[maxRow][pivot])) maxRow = r;
            // Échanger les lignes pivot et maxRow
            for (int c = 0; c < 8; c++) {
                float tmp = aug[pivot][c];
                aug[pivot][c] = aug[maxRow][c];
                aug[maxRow][c] = tmp;
            }
            float div = aug[pivot][pivot];
            if (fabsf(div) < 1e-8f) return identity(); // matrice singulière
            // Normaliser la ligne pivot
            for (int c = 0; c < 8; c++) aug[pivot][c] /= div;
            // Éliminer la colonne dans toutes les autres lignes
            for (int r = 0; r < 4; r++) {
                if (r == pivot) continue;
                float factor = aug[r][pivot];
                for (int c = 0; c < 8; c++) aug[r][c] -= factor * aug[pivot][c];
            }
        }
        // Extraire la partie droite [I | M⁻¹] et reconvertir en column-major
        NkMat4x4 result;
        for (int r = 0; r < 4; r++)
            for (int c = 0; c < 4; c++)
                result.m[c][r] = aug[r][c+4];
        return result;
    }

    // -------------------------------------------------------------------------
    // Accès aux données
    // -------------------------------------------------------------------------

    // Pointeur vers les données pour glUniformMatrix4fv(..., GL_FALSE, mat.data())
    const float* data() const { return &m[0][0]; }
          float* data()       { return &m[0][0]; }

    // Affichage en représentation mathématique (lignes × colonnes)
    void print() const {
        printf("NkMat4x4:\n");
        for (int r = 0; r < 4; r++) {
            printf("  | %8.4f %8.4f %8.4f %8.4f |\n",
                   m[0][r], m[1][r], m[2][r], m[3][r]);
        }
    }
};
