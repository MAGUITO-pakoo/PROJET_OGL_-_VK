#include "NKWindow/Core/NkMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Events/NkEventSystem.h"
#include "NKRenderer/NkContextDesc.h"
#include "NKRenderer/NkContextFactory.h"
#include "glad/gl.h"

using namespace nkentseu;

int nkmain(const NkEntryState& state) {

    NkWindowConfig wc{};
    wc.title    = "Rectangle avec EBO";
    wc.width    = 1280;
    wc.height   = 720;
    wc.centered = true;

    NkWindow window;
    if (!window.Create(wc)) return -1;

    NkContextDesc desc = NkContextDesc::MakeOpenGL(3, 3, true);
    auto ctx = NkContextFactory::Create(desc, window);
    if (!ctx || !ctx->IsValid()) return -2;


    // ======================================================
    //  VBO + EBO + VAO + Shaders ici
    // ======================================================
    // 4 vertices uniques (position + couleur)
    float vertices[] = {
    //   x      y      z      r     g     b
        -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // A(0) bas-gauche  = rouge
        0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // B(1) bas-droit   = vert
        0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // C(2) haut-droit  = bleu
        -0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f,  // D(3) haut-gauche = jaune
    };

    // 6 indices : 2 triangles, chacun référence 3 vertices par leur numéro
    unsigned int indices[] = {
        0, 1, 3,   // Triangle ① : A, B, D
        1, 2, 3,   // Triangle ② : B, C, D
    };
    unsigned int vbo, ebo, vao;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);         // ← nouveau : générer l'EBO

    glBindVertexArray(vao);        // le VAO enregistre tout ce qui suit

    // VBO : données des vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO : tableau d'indices — slot différent du VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Description des attributs (identique au triangle)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
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

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, nullptr);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fs);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);




    while (window.IsOpen()) {
        NkEvents().PollEvents();
        NkEvent* ev = nullptr;
        while ((ev = NkEvents().PollEvent()) != nullptr) {
            if (ev->Is<NkWindowCloseEvent>()) window.Close();
        }

        if (!ctx->BeginFrame()) continue;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ======================================================
        //  glDrawElements ici
        // ======================================================
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //             ↑              ↑   ↑                ↑
        //         primitive    nb indices  type des indices  offset dans l'EBO


        ctx->EndFrame();
        ctx->Present();
    }

    window.Close();
    return 0;
}
