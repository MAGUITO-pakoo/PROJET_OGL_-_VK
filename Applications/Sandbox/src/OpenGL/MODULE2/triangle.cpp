#include "NKWindow/Core/NkMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Events/NkEventSystem.h"
#include "NKRenderer/NkContextDesc.h"
#include "NKRenderer/NkContextFactory.h"
#include "glad/gl.h"
#include "NKTime/NkChrono.h"
#include "NKMath/NKVec3f.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {

    // --- Fenêtre ---
    NkWindowConfig wc{};
    wc.title    = "Premier Triangle";
    wc.width    = 1280;
    wc.height   = 720;
    wc.resizable = true;
    wc.centered  = true;

    NkWindow window;
    if (!window.Create(wc)) return -1;

    // --- Contexte OpenGL 3.3 Core ---
    NkContextDesc desc = NkContextDesc::MakeOpenGL(3, 3, true);
    auto ctx = NkContextFactory::Create(desc, window);
    if (!ctx || !ctx->IsValid()) return -2;

    // ======================================================
    //  ICI on va ajouter les données, VBO, VAO, shaders...
    // ======================================================
    // ============================================================
    // ÉTAPE 2 : Données des vertices (CPU → tableau en RAM)
    // ============================================================
    float vertices[] = {
//   position              couleur
//   x      y      z       r     g     b
    -0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,  // sommet 0 : rouge
     0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  // sommet 1 : vert
     0.0f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  // sommet 2 : bleu
    };
    // ============================================================
    // ÉTAPE 3 : VBO — envoyer les données vers la VRAM du GPU
    // ============================================================

    unsigned int vbo;
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    //           ↑   ↑
    //    combien    adresse où stocker l'identifiant généré
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ============================================================
    // ÉTAPE 4 : VAO — décrire la structure des données
    // ============================================================

    // Attribut 0 : position (x, y, z)
    glVertexAttribPointer(
    0,          // location = 0  (correspondra à "layout(location=0)" dans le shader)
    3,          // 3 composantes (x, y, z)
    GL_FLOAT,   // type de chaque composante
    GL_FALSE,   // normaliser ? non (nos valeurs sont déjà entre -1 et +1)
    6 * sizeof(float),  // stride : distance entre le début d'un vertex et le suivant
    (void*)0            // offset : où commence cet attribut dans un vertex (octet 0)
    );
    glEnableVertexAttribArray(0);  // activer l'attribut 0

    // Attribut 1 : couleur
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);  
    // ============================================================
    // ÉTAPE 5a : Vertex Shader
    // ============================================================
    const char* vertexShaderSource = R"(
    #version 330 core

    layout(location = 0) in vec3 aPosition;
    layout(location = 1) in vec3 aColor;

    out vec3 vColor;

    void main() {
        gl_Position = vec4(aPosition, 1.0);
        vColor = aColor;
    }
    )"; 
    // ============================================================
    // ÉTAPE 5b : Fragment Shader
    // ============================================================
    const char* fragmentShaderSource = R"(
        #version 330 core

        in vec3 vColor;
        out vec4 FragColor;

        void main() {
            FragColor = vec4(vColor, 1.0);
        }
        )";
    // ============================================================
    // ÉTAPE 5c : Compilation et linkage
    // ============================================================

    // --- Vertex Shader ---
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // --- Fragment Shader ---
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // --- Programme ---
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // --- Nettoyage : les shaders compilés ne sont plus nécessaires ---
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    NkChrono chrono;

    while (window.IsOpen()) {

        // --- Événements ---
        NkEvents().PollEvents();
        NkEvent* ev = nullptr;
        while ((ev = NkEvents().PollEvent()) != nullptr) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
            
        }

        if (!ctx->BeginFrame()) continue;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ======================================================
        //  ICI on va appeler glDrawArrays(...)
        // ======================================================
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //           ↑              ↑  ↑
        //    primitive          début  nombre de vertices

        ctx->EndFrame();
        ctx->Present();
    }

    window.Close();
    return 0;
}