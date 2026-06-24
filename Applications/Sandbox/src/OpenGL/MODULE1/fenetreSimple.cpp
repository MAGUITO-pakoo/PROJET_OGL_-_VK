#include "NKWindow/Core/NkMain.h" // fonction d'entrer
#include "NKWindow/Core/NkWindow.h"// creer la fenetre 
#include "NKWindow/Events/NkEventSystem.h" // re cuperer les evenemnt (souris, clavier..)


using namespace nkentseu;
 int nkmain(const NkEntryState& state) {

    NkWindowConfig  wc{};
    wc.title = "FENETRE VIDE";
    wc.width = 1280;
    wc.height = 720;
    wc.resizable = true;
    wc.centered = true;

    NkWindow window;

    if (!window.Create(wc)){
        return -2;
    }
    while (window.IsOpen()){
        NkEvents().PollEvents();

        NkEvent* ev = nullptr;
        while ((ev = NkEvents().PollEvent()) != nullptr) {
            if (ev->Is<NkWindowCloseEvent>()){
                window.Close();
                break;
            }
        }
    }
     return 0;
 }
