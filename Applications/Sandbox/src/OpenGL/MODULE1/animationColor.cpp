#include "NKWindow/Core/NkMain.h" // fonction d'entrer
#include "NKWindow/Core/NkWindow.h"// creer la fenetre 
#include "NKWindow/Events/NkEventSystem.h" // re cuperer les evenemnt (souris, clavier..)
#include "NKRenderer/NkContextDesc.h"  // specification du contexte opengl a creer (opengl 3.3)
#include "NKRenderer/NkContextFactory.h"// creation du contexte opengl
#include "glad/gl.h"// nettoyer, couleur..
#include "NKTime/NkChrono.h"


using namespace nkentseu;
 int nkmain(const NkEntryState& state) {

    NkWindowConfig  wc{};
    wc.title = "LIVRABLE MODULE1";
    wc.width = 1280;
    wc.height = 720;
    wc.resizable = true;
    wc.centered = true;

    NkWindow window;

    if (!window.Create(wc)){
        return -2;
    }
    // creaction du contexte openGL
    NkContextDesc desc = NkContextDesc::MakeOpenGL(3, 3, true); 
    auto ctx = NkContextFactory::Create(desc, window);
    if (!ctx || !ctx->IsValid()) {
        return -3;
    }

    NkChrono chrono;


    while (window.IsOpen()){
        NkEvents().PollEvents();

        NkEvent* ev = nullptr;
        while ((ev = NkEvents().PollEvent()) != nullptr) {
            if (ev->Is<NkWindowCloseEvent>()){
                window.Close();
                break;
            }
        }
    
    if (!ctx->BeginFrame()) continue;
    /* glClearColor(0.2f, 0.3f, 0.8f, 1.0f); */
    float t = (float)chrono.Elapsed().seconds;
    float r = 0.5f + 0.5f * std::sin(t);
    float g = 0.5f + 0.5f * std::sin(t + 2.0f);
    float b = 0.5f + 0.5f * std::sin(t + 4.0f);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ctx->EndFrame();
    ctx->Present();

    }
    window.Close();

    return 0;
 }
