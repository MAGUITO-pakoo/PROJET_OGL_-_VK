# INVENTAIRE DU PROJET NKENTSEU
> Audit technique réalisé le 2026-06-09 avant démarrage du PROJET_MOTEUR.
> **Aucun fichier modifié** — lecture seule.

---

## 1. NKMATH — État des classes

### Architecture : deux familles parallèles

> **ATTENTION** : Il existe deux implémentations parallèles de NKMath.
> Elles définissent parfois les mêmes noms (`NkVec2f`, `NkVec3f`) dans des contextes
> différents, ce qui **causera des conflits de redéfinition** si incluses ensemble.

| Famille | Fichiers | Namespace | Types primitifs |
|---------|----------|-----------|-----------------|
| **Exercices** (écrits manuellement) | `NKVec2f.h`, `NKVec3f.h`, `NKVec4f.h`, `NKMath2x2.h`, `NKMath3x3.h`, `NKMath.4x4.h` | aucun (global) | `float` brut |
| **Production** (framework) | `NkTypes.h`, `NkFunctions.h` | `nkentseu::` / `nkentseu::math::` | `float32`, `float64` (typedef) |

---

### 1.1 Famille Exercices

#### `NkVec2i` — `NKVec2i.h`
| Méthode | État |
|---------|------|
| `NkVec2i(int x, int y)` | ✅ Implémentée |
| `add(NkVec2i)` | ✅ Implémentée |
| `sub(NkVec2i)` | ✅ Implémentée |
| `scale(int s)` | ✅ Implémentée |
| `print()` | ✅ Implémentée |
| `dot()`, `length()`, `normalize()` | ❌ Absentes |

#### `NkVec2f` — `NKVec2f.h`
| Méthode | État |
|---------|------|
| `NkVec2f(float x, float y)` | ✅ Implémentée |
| `add(NkVec2f)` | ✅ Implémentée |
| `sub(NkVec2f)` | ✅ Implémentée |
| `scale(float)` | ✅ Implémentée |
| `dot(NkVec2f)` | ✅ Implémentée |
| `length()` | ✅ Implémentée |
| `normalize()` | ✅ Implémentée |
| `print()` | ✅ Implémentée |

#### `NkVec3f` — `NKVec3f.h`
| Méthode | État |
|---------|------|
| `NkVec3f(float x, float y, float z)` | ✅ Implémentée |
| `add(NkVec3f)` | ✅ Implémentée |
| `sub(NkVec3f)` | ✅ Implémentée |
| `scale(float)` | ✅ Implémentée |
| `dot(NkVec3f)` | ✅ Implémentée |
| `length()` | ✅ Implémentée |
| `normalize()` | ✅ Implémentée |
| `cross(NkVec3f)` | ✅ Implémentée |
| `print()` | ✅ Implémentée |

#### `NkVec4f` — `NKVec4f.h`
| Méthode | État |
|---------|------|
| `NkVec4f(float x, float y, float z, float w)` | ✅ Implémentée |
| `add(NkVec4f)` | ✅ Implémentée |
| `sub(NkVec4f)` | ✅ Implémentée |
| `scale(float)` | ✅ Implémentée |
| `dot(NkVec4f)` | ✅ Implémentée |
| `length()` | ✅ Implémentée |
| `normalize()` | ✅ Implémentée |
| `print()` | ✅ Implémentée |

> Note : `NKVec4f.h` et `NKMath.4x4.h` redéfinissent tous les deux `NkVec4f`. Même problème entre `NKMath2x2.h`/`NKMath3x3.h` et leurs vecteurs respectifs.

#### `Mat2x2` — `NKMath2x2.h`
| Méthode | État |
|---------|------|
| `Mat2x2(a,b,c,d)` | ✅ Implémentée |
| `print()` | ✅ Implémentée |
| `add(Mat2x2)` | ✅ Implémentée |
| `transpose()` | ✅ Implémentée |
| `determinant()` | ✅ Implémentée |
| `mulVec(NkVec2f)` | ✅ Implémentée |
| `mulMat(Mat2x2)` | ✅ Implémentée |
| `identity()` | ❌ Absente |
| `inverse()` | ❌ Absente |

#### `Mat3x3` — `NKMath3x3.h`
| Méthode | État |
|---------|------|
| `Mat3x3(9 floats)` | ✅ Implémentée |
| `print()` | ✅ Implémentée |
| `add(Mat3x3)` | ✅ Implémentée |
| `transpose()` | ✅ Implémentée |
| `determinant()` (Sarrus) | ✅ Implémentée |
| `mulVec(NkVec3f)` | ✅ Implémentée |
| `mulMat(Mat3x3)` | ✅ Implémentée |
| `identity()` | ❌ Absente |
| `inverse()` | ❌ Absente |

#### `Mat4x4` — `NKMath.4x4.h`
> Nom de fichier inhabituel (point au lieu d'underscore).
> Le struct s'appelle `Mat4x4` (pas `NkMat4x4`).
> Stockage `data[row][col]` → **row-major** (NON compatible avec `glUniformMatrix4fv` sans transposition).

| Méthode | État |
|---------|------|
| `Mat4x4(16 floats)` | ✅ Implémentée |
| `identity()` (static) | ✅ Implémentée |
| `print()` | ✅ Implémentée |
| `transpose()` | ✅ Implémentée |
| `add(Mat4x4)` | ✅ Implémentée |
| `mulVec(NkVec4f)` | ✅ Implémentée |
| `mulMat(Mat4x4)` | ✅ Implémentée |
| **`translation(tx, ty, tz)`** | ❌ **ABSENTE** |
| **`scale(sx, sy, sz)`** | ❌ **ABSENTE** |
| **`rotation(axis, angle)`** | ❌ **ABSENTE** |
| **`perspective(fov, aspect, near, far)`** | ❌ **ABSENTE** |
| **`lookAt(eye, center, up)`** | ❌ **ABSENTE** |
| **`toFloatPtr()`** | ❌ **ABSENTE** (nécessaire pour `glUniformMatrix4fv`) |
| `inverse()` | ❌ Absente |

---

### 1.2 Famille Production (`NkTypes.h` + `NkFunctions.h`)

#### `NkVec2f`, `NkVec2d` — dans `nkentseu::` (`NkTypes.h`)
Opérateurs `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `==`, `!=` → ✅ Implémentés inline
| Méthode | État |
|---------|------|
| `LengthSq()` | ✅ Implémentée inline |
| `Length()` | ✅ Implémentée inline (appelle `sqrt`) |
| `Normalized()` | ✅ Implémentée inline |
| `Dot(NkVec2f)` | ✅ Implémentée inline |

#### `NkVec2i`, `NkVec2u` — dans `nkentseu::` (`NkTypes.h`)
Opérateurs arithmétiques de base → ✅ Implémentés inline

#### `NkVec3f`, `NkVec3d` — dans `nkentseu::` (`NkTypes.h`)
> Version minimale — seulement `+` et `*s`. **Pas de dot, cross, normalize, length.**
| Méthode | État |
|---------|------|
| `operator+` | ✅ |
| `operator*` (scalaire) | ✅ |
| `ToVec2()` | ✅ |
| `dot`, `cross`, `normalize`, `length` | ❌ **ABSENTES** |

#### `NkRect` — dans `nkentseu::` (`NkTypes.h`)
| Méthode | État |
|---------|------|
| `Contains(px, py)` | ✅ Implémentée inline |

#### `NkFunctions.h` — dans `nkentseu::math::`
| Groupe | État |
|--------|------|
| Constantes (π, e, etc.) | ✅ Toutes `constexpr` |
| `NkToRadians`, `NkToDegrees` | ✅ Inline implémentées |
| `NkMin`, `NkMax`, `NkClamp`, `NkSaturate` | ✅ Templates inline |
| `NkAbs`, `NkFabs`, `NkIsNearlyZero`, `NkNearlyEqual` | ✅ Inline |
| `NkLerp`, `NkMix` | ✅ Templates inline |
| `NkTan`, `NkLog10`, `NkLog2` | ✅ Inline (dépendent de NkSin/NkCos/NkLog) |
| `NkIsFinite` | ✅ Inline bit-tricks |
| `NkFloor`, `NkCeil`, `NkTrunc`, `NkRound` | ⚠️ **Déclarées seulement** (corps dans un .cpp compilé) |
| `NkSqrt`, `NkRsqrt`, `NkCbrt` | ⚠️ **Déclarées seulement** |
| `NkExp`, `NkLog`, `NkPow`, `NkPowInt` | ⚠️ **Déclarées seulement** |
| `NkSin`, `NkCos`, `NkAtan`, `NkAtan2`, `NkAsin`, `NkAcos` | ⚠️ **Déclarées seulement** |
| `NkSinh`, `NkCosh`, `NkTanh` | ⚠️ **Déclarées seulement** |
| `NkSmoothstep`, `NkSmootherstep` | ⚠️ **Déclarées seulement** |
| `NkGcd`, `NkNextPowerOf2`, `NkClz`, `NkCtz`, `NkPopcount` | ⚠️ **Déclarées seulement** |

> Les fonctions "déclarées seulement" sont implémentées dans des fichiers `.cpp` compilés dans la bibliothèque `NKMath.lib`. Elles sont utilisables à condition de linker correctement.

---

### 1.3 Types complètement absents (nécessaires pour le Projet Moteur)

| Type | Utilité | Priorité |
|------|---------|----------|
| `NkMat4x4` (version OpenGL) | MVP transforms, `glUniformMatrix4fv` | 🔴 Critique |
| `NkQuaternion` | Rotations 3D, caméra | 🟠 Haute |
| `NkTransform3D` | Position + rotation + scale → matrix | 🟠 Haute |
| `NkColorF` / `NkColor` | Couleurs shader | 🟡 Moyenne |

---

## 2. NKWINDOW — État opérationnel

### 2.1 Création de fenêtre + contexte OpenGL 3.3 Core

**Statut : ✅ Fonctionnel et prouvé**

Le pipeline `NkWindowConfig → NkWindow::Create() → NkContextDesc::MakeOpenGL(3,3,true) → NkContextFactory::Create()` est implémenté et testé dans MODULE1 et MODULE2.

```
NkContextDesc::MakeOpenGL(major, minor, coreProfile)  ✅ Helper disponible
NkContextFactory::Create(desc, window)                ✅ Implémenté (NkContextFactory.cpp)
ctx->IsValid()                                        ✅
ctx->BeginFrame()                                     ✅
ctx->EndFrame()                                       ✅
ctx->Present()        → swap buffers                  ✅
```

Backends OpenGL disponibles : `NkOpenGLContext` (Win32 via WGL, Linux via GLX, macOS via NSOpenGL).

### 2.2 NkEventSystem — Clavier et Souris

**Statut : ✅ Opérationnel**

Architecture : ring buffer dual-priorité
- File **HIGH** (128 slots, jamais droppée) : window lifecycle, clavier, clics souris, gamepad
- File **NORMAL** (512 slots, drop-oldest) : mouse move, analog, touch

| Fonctionnalité | État |
|---------------|------|
| `NkEvents().PollEvents()` | ✅ |
| `NkEvents().PollEvent()` → `NkEvent*` | ✅ |
| `ev->Is<NkWindowCloseEvent>()` | ✅ |
| `NkKeyboardEvent` (HIGH priority) | ✅ |
| `NkMouseEvent` (move = NORMAL, click = HIGH) | ✅ |
| `NkMouseMoveEvent` | ✅ (nécessaire caméra FPS) |
| `AddEventCallbackGuard<T>()` → RAII | ✅ |
| Gamepad | ✅ |

**Utilisation clavier typique pour PROJET_MOTEUR :**
```cpp
while ((ev = NkEvents().PollEvent()) != nullptr) {
    if (ev->Is<NkKeyboardEvent>()) {
        auto* ke = ev->As<NkKeyboardEvent>();
        // ke->key, ke->pressed, ke->released, ke->repeat
    }
    if (ev->Is<NkMouseMoveEvent>()) {
        auto* mm = ev->As<NkMouseMoveEvent>();
        // mm->x, mm->y, mm->deltaX, mm->deltaY
    }
}
```

---

## 3. APPLICATIONS/SANDBOX/SRC/OPENGL — État des modules

> **Note importante** : Le CLAUDE.md référence des fichiers `main.cpp` pour MODULE1 et MODULE2.
> Ces fichiers **n'existent pas**. Les livrables réels ont des noms différents.

| Fichier réel | Ce que fait CLAUDE.md | Statut |
|-------------|----------------------|--------|
| `MODULE1/animationColor.cpp` | Livrable MODULE1 (fond animé sin) | ✅ Terminé |
| `MODULE1/fenetreSimple.cpp` | Exercice fenêtre sans OpenGL | ✅ Terminé |
| `MODULE2/triangle.cpp` | Livrable MODULE2 (triangle RGB) | ✅ Terminé |
| `MODULE2/rectangle.cpp` | Bonus MODULE2 (rectangle EBO) | ✅ Terminé, **actif dans Jenga** |

### rectangle.cpp — Analyse technique (fichier le plus à jour)

Le fichier `MODULE2/rectangle.cpp` est le plus avancé techniquement :
- ✅ Fenêtre 1280×720 + contexte OpenGL 3.3 Core
- ✅ VBO + EBO + VAO correctement initialisés
- ✅ Vertex shader + fragment shader GLSL compilés inline
- ✅ Attributs position (loc=0) et couleur (loc=1) avec stride/offset corrects
- ✅ `glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)`
- ✅ Gestion `NkWindowCloseEvent`
- ⚠️ Pas de vérification des erreurs de compilation shader (`glGetShaderiv`)
- ⚠️ Pas de nettoyage des ressources GL à la fin (`glDeleteVertexArrays`, etc.)
- ⚠️ Pas de `glEnable(GL_DEPTH_TEST)` (inutile en 2D mais absent pour 3D)

---

## 4. RÉCAPITULATIF — Ce qu'il faut créer pour PROJET_MOTEUR

### Priorité 1 — Fondation mathématique (bloque tout le reste)
- [ ] `NkMat4x4` complète : `identity`, `translation`, `scale`, `rotation(axis,angle)`, `perspective`, `lookAt`, `mulMat`, `mulVec`, `transpose`, `toFloatPtr()` — **column-major pour OpenGL**
- [ ] `NkQuaternion` : `fromAxisAngle`, `toMat4`, `slerp`, `normalize`, `mul`
- [ ] `NkTransform3D` : position + rotation + scale → `NkMat4x4`

### Priorité 2 — Abstraction GPU (évite code répété)
- [ ] `ShaderProgram` : compile, link, `setUniformMat4`, `setUniformVec3`, `setUniformFloat`, `setUniform1i`
- [ ] `Mesh` : encapsule VAO/VBO/EBO, `draw()`

### Priorité 3 — Fonctionnalités moteur
- [ ] `Texture2D` : chargement stb_image, upload GPU, bind unit
- [ ] `Camera` : FPS (position + yaw/pitch → viewMatrix)
- [ ] `FrameBuffer` : FBO + color attachment + depth RBO

### Priorité 4 — Rendu avancé
- [ ] Éclairage Phong en GLSL (3 types de lumières)
- [ ] 2 effets post-processing (FBO → fullscreen quad)
- [ ] Scene graph basique (Node + world matrix)

### Dossier cible
```
Applications/Sandbox/src/PROJET_MOTEUR/
├── main.cpp          ← point d'entrée, intègre tout
├── ShaderProgram.h
├── Mesh.h
├── Texture2D.h
├── Camera.h
└── FrameBuffer.h
```

---

## 5. AVERTISSEMENTS TECHNIQUES

1. **Conflits de redéfinition NKMath** : les fichiers `NKVec2f.h`, `NKMath2x2.h`, `NKMath3x3.h`, `NKMath.4x4.h` redéfinissent des structs déjà présents dans `NkTypes.h`. Ne jamais les inclure ensemble dans un même fichier compilé.

2. **Row-major vs Column-major** : la `Mat4x4` existante stocke `data[row][col]` (row-major). OpenGL veut du column-major. La nouvelle `NkMat4x4` du projet moteur devra utiliser `m[col][row]` ou passer `GL_TRUE` à `glUniformMatrix4fv` pour transposer.

3. **Nom de fichier `NKMath.4x4.h`** : le point dans le nom peut causer des problèmes selon le compilateur/build system. À inclure avec `#include "NKMath/NKMath.4x4.h"` — ça fonctionne sous MSVC/GCC mais reste inhabituel.

4. **`NkFunctions.h`** : les fonctions trigonométriques (`NkSin`, `NkCos`, etc.) sont seulement déclarées dans le header. Elles fonctionnent via le link avec `NKMath`. Pour le PROJET_MOTEUR, utiliser directement `<cmath>` (`sinf`, `cosf`) dans les fichiers autonomes pour éviter une dépendance au link.

5. **Pas de `main.cpp` dans MODULE1/MODULE2** : le CLAUDE.md les référence mais ils n'existent pas. Les fichiers réels sont `animationColor.cpp` et `triangle.cpp`.
