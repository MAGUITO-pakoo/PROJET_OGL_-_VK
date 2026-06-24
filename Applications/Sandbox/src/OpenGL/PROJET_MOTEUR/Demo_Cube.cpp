#include "NKWindow/Core/NkMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Events/NkEventSystem.h"
#include "NKRenderer/NkContextDesc.h"
#include "NKRenderer/NkContextFactory.h"
#include "NKRenderer/Mesh.h"
#include "NKRenderer/ShaderProgram.h"
#include "NKRenderer/Texture2D.h"
#include "NKTime/NkChrono.h"
#include "NKMath/NkMat4x4.h"
#include "NKMath/NKVec3f.h"
#include "NKMath/NKVec2f.h"
#include "glad/gl.h"
#include <filesystem>
#include <iostream>

using namespace nkentseu;

static std::string ResolveShaderPath(const char* filename) {
    std::filesystem::path currentFile(__FILE__);
    std::filesystem::path shaderPath = currentFile.parent_path() / filename;
    return shaderPath.string();
}

static std::string ResolveTexturePath(const char* filename) {
    std::filesystem::path currentFile(__FILE__);
    std::filesystem::path sandboxRoot = currentFile.parent_path().parent_path().parent_path().parent_path();
    return (sandboxRoot / "assets" / "textures" / filename).string();
}

static void CalculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    for (uint32_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i + 0]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        ::NkVec3f edge1 = v1.position - v0.position;
        ::NkVec3f edge2 = v2.position - v0.position;
        ::NkVec2f deltaUV1 = v1.uv - v0.uv;
        ::NkVec2f deltaUV2 = v2.uv - v0.uv;

        float det = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
        float r = det != 0.0f ? 1.0f / det : 0.0f;

        ::NkVec3f tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r;

        v0.tangent = v0.tangent + tangent;
        v1.tangent = v1.tangent + tangent;
        v2.tangent = v2.tangent + tangent;
    }

    for (auto& vertex : vertices) {
        vertex.tangent = vertex.tangent.normalize();
    }
}

int nkmain(const NkEntryState& state) {
    NkWindowConfig wc{};
    wc.title     = "Cube Mesh Demo";
    wc.width     = 1280;
    wc.height    = 720;
    wc.resizable = true;
    wc.centered  = true;

    NkWindow window;
    if (!window.Create(wc)) {
        std::cerr << "Erreur: impossible de créer la fenêtre" << std::endl;
        return -1;
    }

    NkContextDesc desc = NkContextDesc::MakeOpenGL(3, 3, true);
    auto ctx = NkContextFactory::Create(desc, window);
    if (!ctx || !ctx->IsValid()) {
        std::cerr << "Erreur: impossible de créer le contexte OpenGL" << std::endl;
        return -2;
    }

    const nkentseu::Vertex vertexArray[] = {
        { ::NkVec3f(-0.75f, -0.75f, -0.75f), ::NkVec3f(-1.f, -1.f, -1.f).normalize(), ::NkVec2f(0.f, 0.f), ::NkVec3f(0.f) },
        { ::NkVec3f( 0.75f, -0.75f, -0.75f), ::NkVec3f( 1.f, -1.f, -1.f).normalize(), ::NkVec2f(1.f, 0.f), ::NkVec3f(0.f) },
        { ::NkVec3f( 0.75f,  0.75f, -0.75f), ::NkVec3f( 1.f,  1.f, -1.f).normalize(), ::NkVec2f(1.f, 1.f), ::NkVec3f(0.f) },
        { ::NkVec3f(-0.75f,  0.75f, -0.75f), ::NkVec3f(-1.f,  1.f, -1.f).normalize(), ::NkVec2f(0.f, 1.f), ::NkVec3f(0.f) },
        { ::NkVec3f(-0.75f, -0.75f,  0.75f), ::NkVec3f(-1.f, -1.f,  1.f).normalize(), ::NkVec2f(0.f, 0.f), ::NkVec3f(0.f) },
        { ::NkVec3f( 0.75f, -0.75f,  0.75f), ::NkVec3f( 1.f, -1.f,  1.f).normalize(), ::NkVec2f(1.f, 0.f), ::NkVec3f(0.f) },
        { ::NkVec3f( 0.75f,  0.75f,  0.75f), ::NkVec3f( 1.f,  1.f,  1.f).normalize(), ::NkVec2f(1.f, 1.f), ::NkVec3f(0.f) },
        { ::NkVec3f(-0.75f,  0.75f,  0.75f), ::NkVec3f(-1.f,  1.f,  1.f).normalize(), ::NkVec2f(0.f, 1.f), ::NkVec3f(0.f) },
    };
    std::vector<nkentseu::Vertex> vertices(vertexArray, vertexArray + sizeof(vertexArray) / sizeof(vertexArray[0]));

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        4, 0, 3, 3, 7, 4,
        1, 5, 6, 6, 2, 1,
        4, 5, 1, 1, 0, 4,
        3, 2, 6, 6, 7, 3,
    };

    CalculateTangents(vertices, indices);
    Mesh cubeMesh(vertices, indices);

    Texture2D diffuse(ResolveTexturePath("brickwall.png"));
    Texture2D normalMap(ResolveTexturePath("brickwall_normal.png"));
    if (!diffuse.IsValid() || !normalMap.IsValid()) {
        std::cerr << "Erreur: impossible de charger les textures" << std::endl;
        return -4;
    }

    ShaderProgram shader(ResolveShaderPath("cube.vert.glsl"), ResolveShaderPath("cube.frag.glsl"));
    if (!shader.IsValid()) {
        std::cerr << "Erreur: le shader n'a pas pu être initialisé" << std::endl;
        return -3;
    }

    glEnable(GL_DEPTH_TEST);

    NkChrono chrono;

    while (window.IsOpen()) {
        NkEvents().PollEvents();
        NkEvent* ev = nullptr;
        while ((ev = NkEvents().PollEvent()) != nullptr) {
            if (ev->Is<NkWindowCloseEvent>()) {
                window.Close();
            }
        }

        if (!ctx->BeginFrame()) {
            continue;
        }

        glViewport(0, 0, wc.width, wc.height);
        glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = static_cast<float>(chrono.Elapsed().seconds);
        ::NkMat4x4 model = ::NkMat4x4::rotation(::NkVec3f(0.f, 1.f, 0.f), time);
        ::NkMat4x4 view = ::NkMat4x4::lookAt(::NkVec3f(3.f, 3.f, 3.f), ::NkVec3f(0.f, 0.f, 0.f), ::NkVec3f(0.f, 1.f, 0.f));
        ::NkMat4x4 projection = ::NkMat4x4::perspective(3.14159265f / 4.f, 1280.f / 720.f, 0.1f, 100.f);

        shader.Use();
        shader.SetUniform("uModel", model);
        shader.SetUniform("uView", view);
        shader.SetUniform("uProjection", projection);
        shader.SetUniform("uDiffuse", 0);
        shader.SetUniform("uNormalMap", 1);
        shader.SetUniform("uLightDir", ::NkVec3f(0.5f, 1.f, 0.3f).normalize());

        diffuse.Bind(0);
        normalMap.Bind(1);
        cubeMesh.Draw();

        ctx->EndFrame();
        ctx->Present();
    }

    window.Close();
    return 0;
}
