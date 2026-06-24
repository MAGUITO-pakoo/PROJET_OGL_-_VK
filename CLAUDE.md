# CLAUDE.md — Journal de Formation OpenGL / Nkentseu

> **Règle absolue** : Ce fichier est le **seul fichier** que Claude est autorisé à modifier ou éditer.
> Toute aide sur le code se fait par explication, suggestion ou génération dans la conversation — jamais par édition directe de fichiers source.

---

## Contexte du Projet

**Objectif** : Devenir experte en programmation graphique OpenGL, en comprenant chaque concept en profondeur depuis la base.

**Cours suivi** : *Programmation Graphique OpenGL — Pipeline GPU, GLSL Shaders & Rendu 3D*
- Auteur : Teuguia Tadjuidje Rodolf Sédéris (CEO – Rihen)
- Durée totale : ~120h — 16 semaines — 13 modules

**Stack technique imposée** :
- Langage : C++17
- Framework : **Nkentseu** (custom C++ framework zero-STL)
- Build system : **Jenga** (Python custom)
- Chargeur OpenGL : **glad**
- Maths : **NKMath** (construite au fil du cours)
- Textures : stb_image.h (autorisé)
- **INTERDIT** : GLM, GLFW, SDL, Qt, SFML

**Dossier de travail OpenGL** : `Applications/Sandbox/src/OpenGL/`
Chaque module a son propre sous-dossier (MODULE1, MODULE2, ...).

---

## Règles de Collaboration

1. Claude ne modifie **que** ce fichier CLAUDE.md.
2. Toutes les réponses sont en **français**.
3. L'apprentissage est **en profondeur** : chaque fonction, chaque concept doit être expliqué et compris avant de passer à la suite.
4. Claude met à jour ce fichier à chaque fin de module ou étape importante.

---

## Architecture du Framework Nkentseu (rappel pour Claude)

### Couches (bas → haut)
```
FOUNDATION : NKPlatform → NKCore → NKMath / NKMemory / NKContainers
SYSTEM     : NKTime / NKLogger / NKThreading / NKFileSystem / NKStream
RUNTIME    : NKWindow → NKRenderer → NKCamera
APPLICATION: Sandbox / Jeux / Outils
```

### Points clés
- **Zéro STL** : tout est réimplémenté (NkString, NkVector, NkHashMap...)
- **Zéro PIMPL** : membres platform-spécifiques dans les headers via `#ifdef`
- **NkWindow** crée la fenêtre ET le contexte OpenGL (Core Profile 3.3+)
- **NkContextFactory** fabrique le contexte GL à partir d'un `NkContextDesc`
- **NkEventSystem** gère tous les événements (clavier, souris, fenêtre)
- **NkChrono** fournit un timer haute précision (QueryPerformanceCounter sur Windows)
- **glad** charge les pointeurs de fonctions OpenGL au runtime

### Squelette d'application type
```cpp
#include "NKWindow/Core/NkMain.h"         // point d'entrée multiplateforme
#include "NKWindow/Core/NkWindow.h"        // création de fenêtre
#include "NKWindow/Events/NkEventSystem.h" // système d'événements
#include "NKRenderer/NkContextDesc.h"      // description du contexte GL
#include "NKRenderer/NkContextFactory.h"   // fabrique du contexte GL
#include "glad/gl.h"                       // fonctions OpenGL
#include "NKTime/NkChrono.h"               // timer

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    // 1. Configurer et créer la fenêtre
    NkWindowConfig wc{};
    wc.title = "..."; wc.width = 1280; wc.height = 720;
    NkWindow window;
    window.Create(wc);

    // 2. Créer le contexte OpenGL 3.3 Core
    NkContextDesc desc = NkContextDesc::MakeOpenGL(3, 3, true);
    auto ctx = NkContextFactory::Create(desc, window);

    // 3. Boucle principale
    while (window.IsOpen()) {
        NkEvents().PollEvents();
        // gérer les événements...
        ctx->BeginFrame();
        // rendu OpenGL...
        ctx->EndFrame();
        ctx->Present();
    }
    window.Close();
    return 0;
}
```

---

## Progression des Modules

### MODULE 1 — Fondations : GPU, OpenGL & Contexte ✅ TERMINÉ
**Durée** : 8h | **Fichier** : `Applications/Sandbox/src/OpenGL/MODULE1/main.cpp`

#### Ce qui a été appris et compris en détail

**Architecture GPU**
- CPU vs GPU : le GPU est un processeur massivement parallèle (milliers de cores simples vs quelques cores complexes CPU)
- Pipeline fixe (OpenGL 1.x) vs pipeline programmable (OpenGL 2.0+) vs Core Profile (3.3+)
- Core Profile = plus d'anciennes API dépréciées (`glBegin/glEnd`, matrices fixes...) — on code tout en shaders
- Compatibility Profile = garde la compatibilité avec l'ancien code (à éviter pour apprendre)
- Drivers OpenGL / ICD (Installable Client Driver) : le driver du GPU implémente la spec OpenGL
- Contexte OpenGL = état global associé à un thread ; un seul thread peut être "owner" à la fois

**Pourquoi glad est nécessaire**
- OpenGL n'est pas une bibliothèque statique liée à la compilation
- Les fonctions GL sont des **pointeurs de fonctions** fournis par le driver au runtime
- `glad` charge ces pointeurs via `wglGetProcAddress` (Windows) ou `glXGetProcAddress` (Linux)
- Sans glad : appeler `glClear()` directement = crash (symbole non résolu)
- `gladLoadGL(...)` doit être appelé APRÈS la création du contexte

**NkContextDesc & NkContextFactory**
```cpp
NkContextDesc desc = NkContextDesc::MakeOpenGL(3, 3, true);
//                                              ^   ^   ^
//                                          major minor coreProfile
auto ctx = NkContextFactory::Create(desc, window);
// NkContextFactory choisit le backend correct selon la plateforme
// ctx est un shared_ptr<NkIContext> (interface)
```
- `NkContextDesc::MakeOpenGL(3, 3, true)` : demande OpenGL 3.3 Core Profile
- `NkContextFactory::Create` : crée le contexte natif (WGL sur Windows, GLX sur Linux)
- `ctx->IsValid()` : vérifie que le contexte a bien été créé
- `ctx->BeginFrame()` : rend le contexte courant pour ce frame (make current)
- `ctx->EndFrame()` : finalise le frame côté OpenGL
- `ctx->Present()` : **swap buffers** — présente le backbuffer à l'écran (double buffering)

**NkWindow**
```cpp
NkWindowConfig wc{};
wc.title = "LIVRABLE MODULE1";
wc.width = 1280;
wc.height = 720;
wc.resizable = true;
wc.centered = true;
NkWindow window;
window.Create(wc);  // crée la fenêtre native (HWND sur Windows)
```
- `window.IsOpen()` : retourne `true` tant que la fenêtre n'est pas fermée
- `window.Close()` : ferme la fenêtre proprement

**NkEventSystem**
```cpp
NkEvents().PollEvents();          // traite tous les événements OS en attente
NkEvent* ev = nullptr;
while ((ev = NkEvents().PollEvent()) != nullptr) {
    if (ev->Is<NkWindowCloseEvent>()) {
        window.Close();           // utilisateur clique sur X
        break;
    }
}
```
- `NkEvents()` : singleton de l'event system
- `PollEvents()` : pompe les messages OS (MSG sur Windows, XEvent sur Linux)
- `PollEvent()` : retourne le prochain événement de la queue, `nullptr` si vide
- `ev->Is<T>()` : test de type RTTI custom (pas de dynamic_cast STL)
- `NkWindowCloseEvent` : déclenché quand l'utilisateur ferme la fenêtre

**NkChrono**
```cpp
NkChrono chrono;                        // démarre le timer au moment de la construction
float t = (float)chrono.Elapsed().seconds; // temps écoulé depuis la construction en secondes
```
- Utilise `QueryPerformanceCounter` sur Windows (~100ns de résolution)
- `Elapsed()` retourne un `NkElapsedTime` avec `.seconds`, `.milliseconds`, `.microseconds`, `.nanoseconds`
- Ne se remet PAS à zéro automatiquement (utiliser `chrono.Reset()` pour ça)

**Fonctions OpenGL utilisées**
```cpp
glClearColor(r, g, b, a);        // définit la couleur d'effacement (état GL)
glClear(GL_COLOR_BUFFER_BIT);    // efface le color buffer avec la couleur définie
```
- `glClearColor` ne nettoie PAS : elle définit juste l'état "couleur d'effacement"
- `glClear` fait le vrai nettoyage — `GL_COLOR_BUFFER_BIT` = nettoyer la couleur
- On peut combiner : `glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)`

**Fond coloré animé (livrable M01)**
```cpp
float t = (float)chrono.Elapsed().seconds;
float r = 0.5f + 0.5f * std::sin(t);
float g = 0.5f + 0.5f * std::sin(t + 2.0f);
float b = 0.5f + 0.5f * std::sin(t + 4.0f);
glClearColor(r, g, b, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);
```
- `sin(t)` oscille entre -1 et +1 → `0.5 + 0.5 * sin(t)` oscille entre 0.0 et 1.0 (plage valide pour une couleur GL)
- Décalage de phase (+2.0f, +4.0f) pour que R, G, B ne soient pas synchronisées → effet arc-en-ciel

#### Code final du livrable M01
Voir : `Applications/Sandbox/src/OpenGL/MODULE1/main.cpp`

**Statut** : Fenêtre OpenGL 1280×720 fonctionnelle avec fond coloré animé (sin du temps). ✅

#### Exercices supplémentaires MODULE 1

**`fenetreSimple.cpp`** — fenêtre sans contexte OpenGL (exercice d'entraînement)
- N'utilise PAS `NkContextFactory` ni `glad` — juste fenêtre + events
- Erreur rencontrée : `window.Close()` placé dans la boucle principale au lieu du handler d'événement → la fenêtre se fermait au frame 1 immédiatement
- Règle : `window.Close()` ne doit être appelé QUE dans le handler `NkWindowCloseEvent`

**`AnimationColor.cpp`** — animation couleur (variante du livrable main.cpp)
- Même principe sin(t) que main.cpp mais dans un fichier séparé
- Pour le lancer : dans `Sandbox.jenga`, mettre comme ligne active :
  ```python
  configure_sandbox_app("Sandbox", "src/OpenGL/MODULE1/AnimationColor.cpp", "sandbox.module1.anim")
  ```

---

### MODULE 2 — Pipeline Graphique & Premier Triangle ✅ TERMINÉ
**Durée** : 8h | **Fichier** : `Applications/Sandbox/src/OpenGL/MODULE2/main.cpp`

#### Ce qui a été appris et compris en détail

**CPU ↔ GPU : deux mondes séparés**
- Le GPU a sa propre mémoire (VRAM) — inaccessible directement depuis le CPU
- Il faut transférer explicitement les données CPU → VRAM avant de dessiner
- C'est le rôle du VBO

**Le système de coordonnées NDC**
- OpenGL ne travaille pas en pixels mais en Normalized Device Coordinates
- Centre = (0,0), bords = -1 et +1 sur chaque axe
- Peu importe la résolution de la fenêtre — OpenGL s'adapte automatiquement

**Le format interleaved**
- Position et couleur entrelacées dans un seul tableau de floats
- `[ x y z r g b | x y z r g b | x y z r g b ]` — 6 floats par vertex = 24 octets
- stride = 24 octets (distance entre deux vertices consécutifs)
- offset position = 0, offset couleur = 12 octets (3 floats × 4 octets)

**VBO — Vertex Buffer Object**
```cpp
unsigned int vbo;
glGenBuffers(1, &vbo);                   // réserve un ID en VRAM
glBindBuffer(GL_ARRAY_BUFFER, vbo);      // "je parle de ce buffer"
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copie CPU→VRAM
```
- `GL_ARRAY_BUFFER` : slot pour données de vertices
- `GL_STATIC_DRAW` : hint au driver — données envoyées une fois, lues souvent
- `GL_DYNAMIC_DRAW` : données modifiées souvent | `GL_STREAM_DRAW` : change chaque frame

**VAO — Vertex Array Object**
```cpp
unsigned int vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);                  // tout ce qui suit est mémorisé dans le VAO

// Attribut 0 : position
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Attribut 1 : couleur
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
glEnableVertexAttribArray(1);

glBindVertexArray(0);                    // débinder — bonne pratique
```
- Le VAO mémorise la description du VBO (layout, stride, offset, type)
- Règle absolue : **Generate → Bind → Configure** — toujours dans cet ordre
- Le VAO doit être bindé AVANT le VBO pour que l'association soit enregistrée
- `glVertexAttribPointer(location, nb_composantes, type, normaliser, stride, offset)`
- `location` doit correspondre au `layout(location=N)` dans le vertex shader

**Shaders GLSL**

Vertex Shader :
```glsl
#version 330 core
layout(location = 0) in vec3 aPosition;  // attribut 0 du VAO
layout(location = 1) in vec3 aColor;     // attribut 1 du VAO
out vec3 vColor;                         // transmis au fragment shader
void main() {
    gl_Position = vec4(aPosition, 1.0);  // position en clip space
    vColor = aColor;
}
```

Fragment Shader :
```glsl
#version 330 core
in vec3 vColor;       // reçu du vertex shader (interpolé automatiquement)
out vec4 FragColor;   // couleur finale du pixel
void main() {
    FragColor = vec4(vColor, 1.0);  // RGB + alpha opaque
}
```

**Compilation et linkage des shaders**
```cpp
// Compiler
unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vs, 1, &vertexShaderSource, nullptr);
glCompileShader(vs);

unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fs, 1, &fragmentShaderSource, nullptr);
glCompileShader(fs);

// Lier
unsigned int program = glCreateProgram();
glAttachShader(program, vs);
glAttachShader(program, fs);
glLinkProgram(program);

// Nettoyer les shaders individuels (intégrés dans le programme, plus utiles)
glDeleteShader(vs);
glDeleteShader(fs);
```
- `nullptr` dans `glShaderSource` = OpenGL détecte la longueur automatiquement
- Après `glLinkProgram`, les shaders individuels peuvent être supprimés

**Draw call**
```cpp
glUseProgram(shaderProgram);          // activer les shaders
glBindVertexArray(vao);               // activer les données
glDrawArrays(GL_TRIANGLES, 0, 3);    // dessiner 3 vertices = 1 triangle
```

**L'interpolation automatique**
- Chaque vertex a une couleur (rouge, vert, bleu)
- Le GPU interpole automatiquement les couleurs pour tous les pixels entre les vertices
- C'est la rasterisation qui fait ça — on n'écrit rien pour l'obtenir

**Erreurs rencontrées et corrigées**
1. `glGenVertexArrays` appelé APRÈS `glBindVertexArray` → VAO non initialisé bindé (UB)
2. Chemin Jenga en double (`src/OpenGL/OpenGL/MODULE2/`) → corriger en `src/OpenGL/MODULE2/`
3. VBO bindé avant VAO → attributs non enregistrés dans le VAO
4. Attribut 1 (couleur) oublié dans `glVertexAttribPointer`

#### Résultat du livrable M02 — Triangle
Triangle RGB 1280×720 avec dégradé interpolé automatiquement par le GPU. ✅
Sommet bas-gauche = rouge | Sommet bas-droit = vert | Sommet haut = bleu

---

#### Bonus M02 — Rectangle avec EBO
**Fichier** : `Applications/Sandbox/src/OpenGL/MODULE2/rectangle.cpp`

**Pourquoi l'EBO existe**
- Un rectangle = 2 triangles = 6 vertices SANS EBO → B et D répétés
- AVEC EBO : 4 vertices uniques + 6 indices → aucune duplication
- Sur un vrai modèle 3D, l'économie est massive (dizaines de Mo)

**Les données**
```cpp
float vertices[] = {
//   x      y      z      r     g     b
    -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // A(0) bas-gauche  = rouge
     0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // B(1) bas-droit   = vert
     0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // C(2) haut-droit  = bleu
    -0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f,  // D(3) haut-gauche = jaune
};
unsigned int indices[] = {
    0, 1, 3,   // Triangle ① : A, B, D
    1, 2, 3,   // Triangle ② : B, C, D
};
```

**VBO + EBO + VAO**
```cpp
unsigned int vbo, ebo, vao;
glGenVertexArrays(1, &vao);
glGenBuffers(1, &vbo);
glGenBuffers(1, &ebo);

glBindVertexArray(vao);

glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);   // slot différent du VBO
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
glEnableVertexAttribArray(1);

glBindVertexArray(0);
// ATTENTION : ne jamais débinder l'EBO AVANT glBindVertexArray(0)
// Le VAO mémorise l'EBO bindé — le débinder avant = VAO perd la référence
```

**Draw call avec EBO**
```cpp
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//             ↑              ↑   ↑               ↑
//         primitive    nb indices  type indices   offset dans l'EBO (0 = depuis le début)
```

**glDrawArrays vs glDrawElements**
```
glDrawArrays(GL_TRIANGLES, 0, 3)
→ lit les vertices SÉQUENTIELLEMENT dans le VBO
→ simple, pas d'EBO, mais répétition si vertices partagés

glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)
→ lit d'abord les INDICES dans l'EBO
→ puis va chercher le vertex correspondant dans le VBO
→ chaque vertex stocké une seule fois, réutilisable autant de fois
```

**L'offset dans glDrawElements**
- Indique à quel octet de l'EBO commencer la lecture
- `0` = depuis le début (cas le plus courant)
- Utile si plusieurs meshes partagent un même EBO (on pointe vers le bon sous-ensemble)

**Ce que ctx est réellement**
- `auto ctx = NkContextFactory::Create(desc, window)` retourne `std::unique_ptr<NkIGraphicsContext>`
- `NkIGraphicsContext` = interface pure (classe abstraite — toutes les méthodes sont `= 0`)
- Le vrai objet derrière = `NkOpenGLContext` (choisi par la factory selon le `desc`)
- Si le desc change (Vulkan, DX12...), le code de boucle ne change pas → polymorphisme
- `unique_ptr` = pointeur intelligent, détruit automatiquement à la fin du scope (pas de `delete`)

**Règle de travail multi-fichiers avec Jenga**
- Le glob `**.cpp` attrape TOUS les .cpp → deux `nkmain` = erreur linker
- Toujours pointer vers UN seul fichier quand on change d'exercice :
  ```python
  configure_sandbox_app("Sandbox", "src/OpenGL/MODULE2/rectangle.cpp", "sandbox.module2.rect")
  ```

**Anatomie de `configure_sandbox_app()` — les 3 arguments**
```python
configure_sandbox_app(
    "Sandbox",                           # 1. project_name  → nom de l'exe + dossiers Build/
    "src/OpenGL/MODULE2/rectangle.cpp",  # 2. source_file   → le seul .cpp compilé
    "sandbox.module2.rect"               # 3. app_id_suffix → ID mobile (ignoré sur desktop)
)
```
- Arg 1 `project_name` : détermine `Build/Bin/Debug-windows/Sandbox/Sandbox.exe` et `Build/Obj/.../Sandbox/`
- Arg 2 `source_file` : le seul `.cpp` que le compilateur reçoit — un seul à la fois (un seul `nkmain`)
- Arg 3 `app_id_suffix` : utilisé uniquement sur Android (`com.nkentseu.<suffix>`) et iOS — **ignoré complètement sur Windows/Linux/macOS**

**Changer de fichier actif dans Jenga**
1. Commenter l'ancienne ligne active avec `#`
2. Écrire (ou décommenter) la nouvelle ligne :
```python
# configure_sandbox_app("Sandbox", "src/OpenGL/MODULE2/rectangle.cpp", "sandbox.module2.rect")  ← commentée
configure_sandbox_app("Sandbox", "src/OpenGL/MODULE1/AnimationColor.cpp", "sandbox.module1.anim")  ← active
```
3. Relancer `jenga run`

**`*.cpp` vs `**.cpp`**
- `*.cpp` → fichiers `.cpp` dans le dossier courant uniquement (pas de sous-dossiers)
- `**.cpp` → récursif — attrape TOUS les `.cpp` dans tous les sous-dossiers
- Dans Jenga, on passe un chemin direct (ex: `"src/OpenGL/MODULE2/rectangle.cpp"`) — on n'utilise pas de glob, donc pas ce problème ici

---

### MODULE 3 — GLSL & Shaders Programmables + Textures & Normal Mapping ✅ TERMINÉ
**Durée** : 16h | **Fichiers** :
- `Applications/Sandbox/src/OpenGL/Final/demo_cube.cpp`
- `Applications/Sandbox/src/OpenGL/Final/cube.vert.glsl`
- `Applications/Sandbox/src/OpenGL/Final/cube.frag.glsl`
- `Modules/Runtime/NKRenderer/src/NKRenderer/Texture2D.h` + `.cpp`
- `Applications/Sandbox/assets/textures/{brickwall.png, brickwall_normal.png}`

#### Concepts appris en détail

**Classe Texture2D — Chargement et gestion GPU**
```cpp
class Texture2D {
    unsigned int mTextureId;
    int mWidth, mHeight, mChannels;
    
public:
    Texture2D(const std::string& filePath);  // Charge via stb_image
    ~Texture2D();                             // Nettoie glDeleteTextures
    bool IsValid() const;
    void Bind(unsigned int unit = 0) const;   // glActiveTexture(GL_TEXTURE0 + unit)
};
```
- **stb_image** : on définit `#define STB_IMAGE_IMPLEMENTATION` AVANT le premier `#include "stb/stb_image.h"` (une seule fois!)
- **Important** : stb_image.h doit être inclus AVANT `<iostream>` car `<iostream>` tire `std::exception` qui peut conflictuer avec les macros internes
- **Upload GPU** :
  ```cpp
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);  // Génère mipmap pyramid
  ```
- **Sampling** :
  ```cpp
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  ```
- **Binding** :
  ```cpp
  glActiveTexture(GL_TEXTURE0 + unit);   // Sélect l'unité (0 = diffuse, 1 = normal, etc.)
  glBindTexture(GL_TEXTURE_2D, mTextureId);
  ```

**Tangent space & TBN matrix**
- Chaque triangle a son propre **tangent** (direction U de la UV) et **bitangent** (direction V)
- Calcul per-triangle :
  ```
  edge1 = v1.pos - v0.pos
  edge2 = v2.pos - v0.pos
  deltaUV1 = v1.uv - v0.uv
  deltaUV2 = v2.uv - v0.uv
  r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x)
  tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r
  ```
- Les tangentes des vertices partagés par plusieurs triangles sont **cumulées** puis **normalisées**
- **TBN matrix** (tangent space → world space) :
  ```glsl
  mat3 TBN = mat3(vTangent, vBitangent, vNormal);
  vec3 worldNormal = normalize(TBN * sampledNormalFromMap);
  ```

**Normal map encoding**
- Normal map stockée en RGB (0..255) → (0..1) dans le shader
- Format standard : `vec3 normal = normalize(texture(map, uv).rgb * 2.0 - 1.0);`
  - Convertit [0,1] → [-1,1]
  - Multiplie par TBN pour passer en world space
  - Normalize pour correction d'artefacts

**Fragment shader avec éclairage**
```glsl
vec3 normal = normalize(vNormal);
vec3 tangent = normalize(vTangent);
vec3 bitangent = normalize(vBitangent);
mat3 TBN = mat3(tangent, bitangent, normal);

vec3 normalSample = texture(uNormalMap, vUV).rgb;
normalSample = normalize(normalSample * 2.0 - 1.0);
vec3 worldNormal = normalize(TBN * normalSample);

vec3 diffuseColor = texture(uDiffuse, vUV).rgb;
float nDotL = max(dot(worldNormal, normalize(uLightDir)), 0.0);
vec3 lighting = diffuseColor * (0.3 + 0.7 * nDotL);

FragColor = vec4(lighting, 1.0);
```

**Résultat** : Cube 512×512 avec relief brique visible (normal map simule rugosité surface). ✅

#### Erreurs rencontrées et corrigées
1. **Macro STB_IMAGE_IMPLEMENTATION placée après `#include <iostream>`** → C++ stdlib tried to parse STL exception headers avant stb_image, conflit macros → **FIX** : stb_image AVANT tout autre header C++
2. **Vecteur tangent unitaire non normalisé** → normale incorrecte au fragment shader → **FIX** : tangent/bitangent/normal toutes normalisés (normalize() dans les shaders ET en C++)
3. **Oubli de tangent layout dans VAO** → tangentes non passées au VS → **FIX** : glVertexAttribPointer(3, ...) avec offset correct

---

### MODULE 4 — Transformations 3D & Matrices MVP 🔜 À VENIR
**Thème** : Espaces de coordonnées, matrices Model/View/Projection
**NKMath** : `NkMat4x4` ✅ complétée | `NkTransform3D` ✅ créée

---

### MODULE 5 — Textures & Sampler Objects 🔜 À VENIR
**Thème** : Upload texture CPU→GPU, mipmaps, filtrage, cubemaps

---

### MODULE 6 — Caméra 3D & Projection 🔜 À VENIR
**Thème** : Caméra FPS, Arcball, frustum culling
**NKMath** : `NkQuaternion` ✅ créée (fromAxisAngle, slerp, toMat4)

---

### MODULE 7 — Éclairage Phong & PBR Intro 🔜 À VENIR
**Thème** : Ambiant/Diffuse/Spéculaire, Cook-Torrance BRDF

---

### MODULE 8 — Scene Graph & Instancing 🔜 À VENIR
**Thème** : Node tree, world matrix, draw calls instanciés

---

### MODULE 9 — Framebuffers & Post-Processing 🔜 À VENIR
**Thème** : FBO, RBO, bloom, HDR, SSAO

---

### MODULE 10 — Géométrie Avancée & Geometry Shaders 🔜 À VENIR
**Thème** : Geometry Shader, tessellation, displacement

---

### MODULE 11 — Compute Shaders & GPGPU 🔜 À VENIR
**Thème** : Compute shaders, SSBO, simulation de particules GPU

---

### MODULE 12 — Techniques Avancées 🔜 À VENIR
**Thème** : Shadow mapping (PCF, CSM), PBR complet, IBL

---

### MODULE 13 — Projet Final 🔜 À VENIR
**Thème** : Moteur 3D OpenGL complet (18h)
**Exigences** : OpenGL 3.3+, NKMath complète, VAO/VBO/EBO, textures, Phong ou PBR, caméra interactive, FBO + 2 post-effets, scene graph + frustum culling, 60 FPS stable 1280×720

---

## État actuel de la progression

| Module / Composant | Fichier | Statut |
|--------------------|---------|--------|
| MODULE1 livrable | `src/OpenGL/MODULE1/animationColor.cpp` | ✅ Terminé |
| MODULE1 exercice | `src/OpenGL/MODULE1/fenetreSimple.cpp` | ✅ Terminé |
| MODULE2 triangle | `src/OpenGL/MODULE2/triangle.cpp` | ✅ Terminé |
| MODULE3 texture | `src/OpenGL/Final/demo_cube.cpp` | ✅ Terminé |
| MODULE3 shaders | `src/OpenGL/Final/{cube.vert,frag}.glsl` | ✅ Terminé |
| Texture2D class | `Modules/Runtime/NKRenderer/src/NKRenderer/Texture2D.{h,cpp}` | ✅ Terminé |
| Brick textures | `Applications/Sandbox/assets/textures/` | ✅ Terminé |
| **NKMath complète** | `Modules/Foundation/NKMath/src/NKMath/` | ✅ Terminé |
| Tests NKMath | `src/Tests/test_nkmath.cpp` | ✅ À exécuter |
| PROJET_MOTEUR | `src/PROJET_MOTEUR/` | 🔜 À démarrer |

**Prochaine session** : Créer `PROJET_MOTEUR/main.cpp` (fenêtre + cube 3D MVP) et `ShaderProgram.h` / `Mesh.h`.

---

## Notes Importantes

- La variable `ctx` retournée par `NkContextFactory::Create` est un `unique_ptr` — elle est automatiquement détruite en fin de scope (RAII).
- `NkMain.h` remplace `main()` standard — il gère le point d'entrée multiplateforme (WinMain sur Windows, main sur Linux). Les fichiers **sans** `NkMain.h` (ex: `test_nkmath.cpp`) utilisent `int main()` standard.
- `using namespace nkentseu;` est utilisé dans tous les fichiers du cours pour éviter le préfixe `nkentseu::` partout.
- Le contexte OpenGL doit être créé **avant** d'appeler `gladLoadGL` ou d'utiliser des fonctions GL.
- Pour changer de projet dans Jenga : éditer `Applications/Sandbox/Sandbox.jenga`, commenter l'ancienne ligne `configure_sandbox_app(...)` et activer la nouvelle, puis relancer `jenga run`.
- Pour lancer les tests NKMath : `configure_sandbox_app("Sandbox", "src/Tests/test_nkmath.cpp", "sandbox.tests.nkmath")` — retourne 0 si tout passe, nombre de FAILs sinon.

---

## NKMath — Référence Rapide (version PROJET_MOTEUR)

**Fichiers dans `Modules/Foundation/NKMath/src/NKMath/`**

### NkVec3f — `NKVec3f.h`
```cpp
NkVec3f a(1,2,3), b(4,5,6);
a + b; a - b; a * 2.f; a / 2.f; -a;  // opérateurs
a += b; a -= b; a *= 2.f;             // opérateurs d'affectation
a.dot(b)        // produit scalaire → float
a.cross(b)      // produit vectoriel → NkVec3f
a.length()      // norme → float
a.normalize()   // vecteur unitaire → NkVec3f
a.reflect(n)    // réflexion par rapport à n (normalisée) → NkVec3f
```

### NkVec4f — `NKVec4f.h` (inclut NKVec3f.h)
```cpp
NkVec4f v(1,2,3,1);               // x,y,z,w
NkVec4f v(NkVec3f(1,2,3), 1.f);   // construction depuis vec3
v.xyz()                            // swizzle → NkVec3f{x,y,z}
v + u; v * s; v.dot(u); v.length(); v.normalize();
```

### NkMat4x4 — `NkMat4x4.h` (column-major, compatible OpenGL)
```cpp
// Constructeurs
NkMat4x4::identity()
NkMat4x4::translation(NkVec3f{tx,ty,tz})
NkMat4x4::scale(NkVec3f{sx,sy,sz})
NkMat4x4::rotation(NkVec3f axis, float angle_radians)
NkMat4x4::perspective(float fovY_rad, float aspect, float near, float far)
NkMat4x4::lookAt(NkVec3f eye, NkVec3f center, NkVec3f up)

// Opérations
A * B             // NkMat4x4 × NkMat4x4
A * v             // NkMat4x4 × NkVec4f
A.transpose()
A.inverse()       // Gauss-Jordan, retourne identity() si singulière

// OpenGL
glUniformMatrix4fv(loc, 1, GL_FALSE, mat.data());
```
**Stockage** : `m[col][row]` — `m[3][0]` = tx, `m[3][1]` = ty, `m[3][2]` = tz

### NkQuaternion — `NkQuaternion.h`
```cpp
NkQuaternion::identity()
NkQuaternion::fromAxisAngle(NkVec3f axis, float angle_rad)
NkQuaternion::slerp(q1, q2, t)   // t ∈ [0,1]
q.toMat4()       // → NkMat4x4 (rotation pure)
q.conjugate()    // rotation inverse
q * p            // composition
q.normalize()
```

### NkTransform3D — `NkTransform3D.h`
```cpp
NkTransform3D t;
t.position = {1, 2, 3};
t.rotation  = NkQuaternion::fromAxisAngle({0,1,0}, angle);
t.scaleVec  = {2, 2, 2};
NkMat4x4 model = t.toMat4();  // TRS : scale → rotation → translation
```

### NkMathUtils — `NkMathUtils.h` (namespace NkMathUtils::)
```cpp
NkMathUtils::PI            // 3.14159...
NkMathUtils::deg2rad(90.f) // → 1.5707...
NkMathUtils::rad2deg(...)
NkMathUtils::lerp(a, b, t)
NkMathUtils::clamp(v, lo, hi)
NkMathUtils::smoothstep(edge0, edge1, x)
```

**AVERTISSEMENT** : Ne pas inclure les anciens fichiers (`NKMath2x2.h`, `NKMath3x3.h`, `NKMath.4x4.h`) avec les nouveaux — ils redéfinissent `NkVec3f` et `NkVec4f` sans `#pragma once` et causeront des erreurs de compilation.
