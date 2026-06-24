// ============================================================
// test_nkmath.cpp — Tests visuels de NKMath pour le moteur OpenGL
// Pas de framework de test : chaque résultat est affiché.
// PASS = la valeur correspond à l'attendu (tolérance 1e-4).
// Lance avec : configure_sandbox_app("Sandbox", "src/Tests/test_nkmath.cpp", ...)
// ============================================================

#include <cstdio>
#include <cmath>

#include <NKMath/NKVec3f.h>
#include <NKMath/NKVec4f.h>
#include <NKMath/NkMat4x4.h>
#include <NKMath/NkQuaternion.h>
#include <NKMath/NkTransform3D.h>
#include <NKMath/NkMathUtils.h>

// -----------------------------------------------------------------------
// Macro de vérification : affiche PASS ou FAIL avec valeurs
// -----------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;
static const float EPS = 1e-4f;

static void check(const char* name, float expected, float got) {
    bool ok = fabsf(expected - got) < EPS;
    if (ok) g_pass++; else g_fail++;
    printf("  [%s] %-38s  expected=%.5f  got=%.5f\n",
           ok ? "PASS" : "FAIL", name, expected, got);
}

static void section(const char* title) {
    printf("\n========== %s ==========\n", title);
}

// -----------------------------------------------------------------------
// Tests NkVec3f
// -----------------------------------------------------------------------
static void test_vec3() {
    section("NkVec3f");

    NkVec3f a(1.f, 2.f, 3.f);
    NkVec3f b(4.f, 5.f, 6.f);

    // Opérateurs
    NkVec3f add = a + b;
    check("add.x", 5.f, add.x);
    check("add.y", 7.f, add.y);
    check("add.z", 9.f, add.z);

    NkVec3f sub = a - b;
    check("sub.x", -3.f, sub.x);

    NkVec3f mul = a * 2.f;
    check("mul.x", 2.f, mul.x);
    check("mul.z", 6.f, mul.z);

    NkVec3f div = b / 2.f;
    check("div.x", 2.f, div.x);

    NkVec3f neg = -a;
    check("neg.x", -1.f, neg.x);

    // dot
    float d = a.dot(b);         // 1*4 + 2*5 + 3*6 = 4+10+18 = 32
    check("dot(a,b)", 32.f, d);

    // cross(X, Y) = Z
    NkVec3f X(1.f, 0.f, 0.f);
    NkVec3f Y(0.f, 1.f, 0.f);
    NkVec3f Z = X.cross(Y);
    check("cross(X,Y).x", 0.f, Z.x);
    check("cross(X,Y).y", 0.f, Z.y);
    check("cross(X,Y).z", 1.f, Z.z);

    // length
    NkVec3f v(3.f, 4.f, 0.f);
    check("length(3,4,0)", 5.f, v.length());

    // normalize
    NkVec3f n = v.normalize();
    check("normalize length", 1.f, n.length());
    check("normalize.x", 0.6f, n.x);
    check("normalize.y", 0.8f, n.y);

    // reflect : vecteur vers le bas (0,-1,0) réfléchi par normale (0,1,0) → (0,1,0)
    NkVec3f incident(0.f, -1.f, 0.f);
    NkVec3f normal  (0.f,  1.f, 0.f);
    NkVec3f ref = incident.reflect(normal);
    check("reflect.x", 0.f,  ref.x);
    check("reflect.y", 1.f,  ref.y);
    check("reflect.z", 0.f,  ref.z);
}

// -----------------------------------------------------------------------
// Tests NkVec4f
// -----------------------------------------------------------------------
static void test_vec4() {
    section("NkVec4f");

    NkVec4f a(1.f, 2.f, 3.f, 4.f);
    NkVec4f b(5.f, 6.f, 7.f, 8.f);

    NkVec4f add = a + b;
    check("add.w", 12.f, add.w);

    NkVec4f mul = a * 3.f;
    check("mul.x", 3.f, mul.x);
    check("mul.w", 12.f, mul.w);

    // dot
    float d = a.dot(b); // 5+12+21+32 = 70
    check("dot(a,b)", 70.f, d);

    // length de (1,0,0,0)
    NkVec4f unit(1.f, 0.f, 0.f, 0.f);
    check("length(1,0,0,0)", 1.f, unit.length());

    // swizzle xyz()
    NkVec3f xyz = a.xyz();
    check("xyz.x", 1.f, xyz.x);
    check("xyz.y", 2.f, xyz.y);
    check("xyz.z", 3.f, xyz.z);

    // Constructeur depuis vec3
    NkVec3f v3(7.f, 8.f, 9.f);
    NkVec4f v4(v3, 1.f);
    check("from_vec3.x", 7.f, v4.x);
    check("from_vec3.w", 1.f, v4.w);
}

// -----------------------------------------------------------------------
// Tests NkMat4x4
// -----------------------------------------------------------------------
static void test_mat4() {
    section("NkMat4x4 — identity & stockage");

    NkMat4x4 I = NkMat4x4::identity();
    // Diagonale = 1
    check("identity[0][0]", 1.f, I.m[0][0]);
    check("identity[1][1]", 1.f, I.m[1][1]);
    check("identity[2][2]", 1.f, I.m[2][2]);
    check("identity[3][3]", 1.f, I.m[3][3]);
    // Hors diagonale = 0
    check("identity[1][0]", 0.f, I.m[1][0]);
    check("identity[0][1]", 0.f, I.m[0][1]);

    section("NkMat4x4 — translation");
    NkMat4x4 T = NkMat4x4::translation({ 3.f, 5.f, 7.f });
    // Colonne 3 doit contenir (3,5,7,1)
    check("T.m[3][0]", 3.f, T.m[3][0]);
    check("T.m[3][1]", 5.f, T.m[3][1]);
    check("T.m[3][2]", 7.f, T.m[3][2]);
    check("T.m[3][3]", 1.f, T.m[3][3]);
    // Appliquer la translation à un point (0,0,0,1) → (3,5,7,1)
    NkVec4f pt = T * NkVec4f(0.f, 0.f, 0.f, 1.f);
    check("T*(0,0,0,1).x", 3.f, pt.x);
    check("T*(0,0,0,1).y", 5.f, pt.y);

    section("NkMat4x4 — scale");
    NkMat4x4 S = NkMat4x4::scale({ 2.f, 3.f, 4.f });
    NkVec4f sv = S * NkVec4f(1.f, 1.f, 1.f, 1.f);
    check("S*(1,1,1).x", 2.f, sv.x);
    check("S*(1,1,1).y", 3.f, sv.y);
    check("S*(1,1,1).z", 4.f, sv.z);
    check("S*(1,1,1).w", 1.f, sv.w);

    section("NkMat4x4 — rotation 90° autour de Y");
    // Rotation 90° autour Y : X→Z, Z→-X
    float angle90 = NkMathUtils::deg2rad(90.f);
    NkMat4x4 R = NkMat4x4::rotation({ 0.f, 1.f, 0.f }, angle90);
    NkVec4f xAxis = R * NkVec4f(1.f, 0.f, 0.f, 0.f); // doit donner (0,0,-1,0)
    check("R90Y*X = (0,0,-1).x",  0.f, xAxis.x);
    check("R90Y*X = (0,0,-1).y",  0.f, xAxis.y);
    check("R90Y*X = (0,0,-1).z", -1.f, xAxis.z);

    section("NkMat4x4 — matrice × matrice");
    // T * S : translation puis scale ne commutent pas
    NkMat4x4 TS = T * S;
    NkVec4f result = TS * NkVec4f(1.f, 1.f, 1.f, 1.f);
    // T*S*(1,1,1,1) : d'abord scale → (2,3,4), puis translation → (5,8,11)
    check("TS*(1,1,1,1).x", 5.f,  result.x);
    check("TS*(1,1,1,1).y", 8.f,  result.y);
    check("TS*(1,1,1,1).z", 11.f, result.z);

    section("NkMat4x4 — transpose");
    NkMat4x4 tT = T.transpose();
    // La ligne 3 de T est {0,0,0,1} → devient la colonne 3 de tT
    // La colonne 3 de T est {3,5,7,1} → devient la ligne 3 de tT
    check("transpose: tT.m[0][3]", 3.f, tT.m[0][3]); // était T.m[3][0]=3
    check("transpose: tT.m[1][3]", 5.f, tT.m[1][3]); // était T.m[3][1]=5

    section("NkMat4x4 — inverse (A * A⁻¹ = I)");
    NkMat4x4 S2 = NkMat4x4::scale({ 2.f, 4.f, 8.f });
    NkMat4x4 invS = S2.inverse();
    NkMat4x4 prod = S2 * invS;
    check("S*invS = I [0][0]", 1.f, prod.m[0][0]);
    check("S*invS = I [1][1]", 1.f, prod.m[1][1]);
    check("S*invS = I [2][2]", 1.f, prod.m[2][2]);
    check("S*invS = I [3][3]", 1.f, prod.m[3][3]);
    check("S*invS = I [0][1]", 0.f, prod.m[0][1]);

    section("NkMat4x4 — perspective (colonne 2 / colonne 3)");
    float fov    = NkMathUtils::deg2rad(60.f);
    float aspect = 1280.f / 720.f;
    NkMat4x4 P = NkMat4x4::perspective(fov, aspect, 0.1f, 100.f);
    // m[2][3] doit être -1 (met w=-z dans l'espace clip)
    check("perspective m[2][3]", -1.f, P.m[2][3]);
    // m[3][3] doit être 0
    check("perspective m[3][3]",  0.f, P.m[3][3]);
    // m[0][0] > 0 et dépend de l'aspect
    float expectedM00 = 1.f / (aspect * tanf(fov * 0.5f));
    check("perspective m[0][0]", expectedM00, P.m[0][0]);

    section("NkMat4x4 — lookAt (vecteur forward après vue)");
    NkVec3f eye   (0.f, 0.f, 3.f);
    NkVec3f center(0.f, 0.f, 0.f);
    NkVec3f up    (0.f, 1.f, 0.f);
    NkMat4x4 V = NkMat4x4::lookAt(eye, center, up);
    // La caméra est en Z=3 qui regarde vers -Z.
    // L'origine (0,0,0,1) doit se retrouver à z=−3 dans view space
    NkVec4f origin = V * NkVec4f(0.f, 0.f, 0.f, 1.f);
    check("lookAt: origin.z en view space", -3.f, origin.z);
    check("lookAt: origin.x en view space",  0.f, origin.x);

    section("NkMat4x4 — data() pour glUniformMatrix4fv");
    const float* ptr = I.data();
    // Éléments [0] et [5] de la diagonale (column-major : index 0=m[0][0], 5=m[1][1])
    check("data()[0]  = 1", 1.f, ptr[0]);
    check("data()[5]  = 1", 1.f, ptr[5]);
    check("data()[10] = 1", 1.f, ptr[10]);
    check("data()[15] = 1", 1.f, ptr[15]);
    check("data()[1]  = 0", 0.f, ptr[1]);
}

// -----------------------------------------------------------------------
// Tests NkQuaternion
// -----------------------------------------------------------------------
static void test_quaternion() {
    section("NkQuaternion — fromAxisAngle");

    // Rotation 0° → quaternion identité
    NkQuaternion q0 = NkQuaternion::fromAxisAngle({ 0.f, 1.f, 0.f }, 0.f);
    check("rot0.x", 0.f, q0.x);
    check("rot0.w", 1.f, q0.w);

    // Rotation 90° autour de Y : w=cos(45°), y=sin(45°)
    float s45 = sinf(NkMathUtils::deg2rad(45.f));
    float c45 = cosf(NkMathUtils::deg2rad(45.f));
    NkQuaternion q90y = NkQuaternion::fromAxisAngle({ 0.f, 1.f, 0.f },
                                                    NkMathUtils::deg2rad(90.f));
    check("q90y.y", s45, q90y.y);
    check("q90y.w", c45, q90y.w);
    check("q90y.x", 0.f, q90y.x);

    section("NkQuaternion — length = 1 après normalize");
    NkQuaternion qn = NkQuaternion(1.f, 2.f, 3.f, 4.f).normalize();
    check("normalized length", 1.f, qn.length());

    section("NkQuaternion — toMat4 cohérent avec NkMat4x4::rotation");
    float angle = NkMathUtils::deg2rad(90.f);
    NkVec3f axis(0.f, 1.f, 0.f);
    NkMat4x4 Rm   = NkMat4x4::rotation(axis, angle);
    NkMat4x4 Rq   = NkQuaternion::fromAxisAngle(axis, angle).toMat4();
    // Les deux matrices doivent être identiques à EPS
    check("Rm vs Rq [0][0]", Rm.m[0][0], Rq.m[0][0]);
    check("Rm vs Rq [0][2]", Rm.m[0][2], Rq.m[0][2]);
    check("Rm vs Rq [2][0]", Rm.m[2][0], Rq.m[2][0]);
    check("Rm vs Rq [2][2]", Rm.m[2][2], Rq.m[2][2]);

    section("NkQuaternion — slerp t=0 donne a, t=1 donne b");
    NkQuaternion qa = NkQuaternion::fromAxisAngle({ 0.f, 1.f, 0.f }, 0.f);
    NkQuaternion qb = NkQuaternion::fromAxisAngle({ 0.f, 1.f, 0.f },
                                                  NkMathUtils::deg2rad(90.f));
    NkQuaternion s0 = NkQuaternion::slerp(qa, qb, 0.f);
    NkQuaternion s1 = NkQuaternion::slerp(qa, qb, 1.f);
    check("slerp t=0 : w=1", 1.f,     s0.w);
    check("slerp t=1 : w=c45", c45,   s1.w);
    check("slerp t=1 : y=s45", s45,   s1.y);

    section("NkQuaternion — conjugate * q = identité");
    NkQuaternion prod = q90y * q90y.conjugate();
    check("q*conj.x", 0.f, prod.x);
    check("q*conj.y", 0.f, prod.y);
    check("q*conj.z", 0.f, prod.z);
    check("q*conj.w", 1.f, prod.w);
}

// -----------------------------------------------------------------------
// Tests NkTransform3D
// -----------------------------------------------------------------------
static void test_transform() {
    section("NkTransform3D — toMat4 TRS");

    // Translation (1,2,3), rotation identité, scale (2,2,2)
    NkTransform3D tr;
    tr.position = { 1.f, 2.f, 3.f };
    tr.rotation  = NkQuaternion::identity();
    tr.scaleVec  = { 2.f, 2.f, 2.f };

    NkMat4x4 model = tr.toMat4();
    // Le point (1,0,0,1) dans l'espace local :
    // après scale(2,2,2)       → (2,0,0,1)
    // après rotation (identité)→ (2,0,0,1)
    // après translation(1,2,3) → (3,2,3,1)
    NkVec4f local(1.f, 0.f, 0.f, 1.f);
    NkVec4f world = model * local;
    check("TRS*(1,0,0,1).x", 3.f, world.x);
    check("TRS*(1,0,0,1).y", 2.f, world.y);
    check("TRS*(1,0,0,1).z", 3.f, world.z);

    // Transform par défaut → matrice identité
    NkTransform3D def;
    NkMat4x4 defM = def.toMat4();
    check("default toMat4 [0][0]", 1.f, defM.m[0][0]);
    check("default toMat4 [3][0]", 0.f, defM.m[3][0]);
}

// -----------------------------------------------------------------------
// Tests NkMathUtils
// -----------------------------------------------------------------------
static void test_mathutils() {
    section("NkMathUtils — constantes & conversions");

    check("PI",          3.14159f, NkMathUtils::PI);
    check("deg2rad(180)", NkMathUtils::PI, NkMathUtils::deg2rad(180.f));
    check("deg2rad(90)",  NkMathUtils::HALF_PI, NkMathUtils::deg2rad(90.f));
    check("rad2deg(PI)",  180.f, NkMathUtils::rad2deg(NkMathUtils::PI));

    section("NkMathUtils — clamp");
    check("clamp(5, 0, 3)",   3.f, NkMathUtils::clamp(5.f, 0.f, 3.f));
    check("clamp(-1, 0, 3)",  0.f, NkMathUtils::clamp(-1.f, 0.f, 3.f));
    check("clamp(2, 0, 3)",   2.f, NkMathUtils::clamp(2.f, 0.f, 3.f));

    section("NkMathUtils — lerp");
    check("lerp(0,10, 0.5)", 5.f, NkMathUtils::lerp(0.f, 10.f, 0.5f));
    check("lerp(0,10, 0.0)", 0.f, NkMathUtils::lerp(0.f, 10.f, 0.0f));
    check("lerp(0,10, 1.0)",10.f, NkMathUtils::lerp(0.f, 10.f, 1.0f));

    section("NkMathUtils — smoothstep");
    check("smoothstep(0,1, 0.0)", 0.f, NkMathUtils::smoothstep(0.f, 1.f, 0.f));
    check("smoothstep(0,1, 1.0)", 1.f, NkMathUtils::smoothstep(0.f, 1.f, 1.f));
    // À t=0.5 : 3*(0.5)²-2*(0.5)³ = 0.75-0.25 = 0.5
    check("smoothstep(0,1, 0.5)", 0.5f, NkMathUtils::smoothstep(0.f, 1.f, 0.5f));
}

// -----------------------------------------------------------------------
// main
// -----------------------------------------------------------------------
int main() {
    printf("============================================================\n");
    printf("  NKMATH — TESTS VISUELS\n");
    printf("============================================================\n");

    test_vec3();
    test_vec4();
    test_mat4();
    test_quaternion();
    test_transform();
    test_mathutils();

    printf("\n============================================================\n");
    printf("  RÉSULTAT : %d PASS  /  %d FAIL\n", g_pass, g_fail);
    printf("============================================================\n");

    if (g_fail > 0) {
        printf("\nDETAIL DES MATRICES (debug) :\n");
        printf("\n-- NkMat4x4::identity() --\n");
        NkMat4x4::identity().print();
        printf("\n-- NkMat4x4::translation(3,5,7) --\n");
        NkMat4x4::translation({ 3.f, 5.f, 7.f }).print();
        printf("\n-- NkMat4x4::rotation(Y, 90°) --\n");
        NkMat4x4::rotation({ 0.f,1.f,0.f }, NkMathUtils::deg2rad(90.f)).print();
        printf("\n-- NkMat4x4::perspective(60°, 16/9, 0.1, 100) --\n");
        NkMat4x4::perspective(NkMathUtils::deg2rad(60.f), 1280.f/720.f, 0.1f, 100.f).print();
    }

    return g_fail;
}
