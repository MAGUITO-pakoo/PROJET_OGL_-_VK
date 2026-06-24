#include "NKWindow/Core/NkMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Events/NkEventSystem.h"
#include "NKRenderer/NkContextDesc.h"
#include "NKRenderer/NkContextFactory.h"
#include <iostream>

using namespace nkentseu;

int nkmain(const NkEntryState& state) {
    NkWindowConfig wc{};
    wc.title     = "FENETRE VIDE";
    wc.width     = 1280;
    wc.height    = 720;
    wc.resizable = true;
    wc.centered  = true;

    NkWindow window;
    if (!window.Create(wc)) {
        std::cerr << "Erreur: impossible de creer la fenetre" << std::endl;
        return -1;
    }
   NkVec2u size = window.GetSize();
   std::cout << "Window size: " << size.x << "x" << size.y << std::endl;

    // Contrairement a OpenGL, pas besoin de PrepareWindowConfig avant Create()
    NkContextDesc desc = NkContextDesc::MakeVulkan(/*validation=*/true);
    auto ctx = NkContextFactory::Create(desc, window);
    if (!ctx || !ctx->IsValid()) {
        std::cerr << "Erreur: impossible de creer le contexte Vulkan" << std::endl;
        return -2;
    }

   while (window.IsOpen()) {
    NkEvents().PollEvents();
    NkEvent* ev = nullptr;
    while ((ev = NkEvents().PollEvent()) != nullptr) {
        if (ev->Is<NkWindowCloseEvent>()) {
            window.Close();
        }
        if (auto* resizeEv = ev->As<NkWindowResizeEvent>()) {
            ctx->OnResize(resizeEv->GetWidth(), resizeEv->GetHeight());
        }
    }

    if (!ctx->BeginFrame()) {
        continue;
    }

    ctx->EndFrame();
    ctx->Present();
   }

    window.Close();
    return 0;
}