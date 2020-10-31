#include "Libraries/SDL/include/SDL.h"

#pragma comment ( lib, "Libraries/SDL/libx64/SDL2.lib" )
#pragma comment ( lib, "Libraries/SDL/libx64/SDL2main.lib" )

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

enum MainStates {
    kMainStart,
    kMainUpdate,
    kMainFinish,
    kMainExit
};

int main(int argc, char** argv) {
    //m_TODO: LOG App Start

    int main_return = EXIT_FAILURE;
    MainStates app_state = kMainStart;

    // -- Application Initialize --
    if (application::Init()) {
        //m_TODO: LOG Starting App Update
        app_state = kMainUpdate;
    }
    else {
        //m_TODO: LOG Failure of Init
        app_state = kMainExit;
    }

    while (app_state == kMainUpdate) {
        app_state = application::Update();
    }

    if (app_state = kMainFinish) {
        if (application::CleanUp())
            main_return = EXIT_SUCCESS;
        else
        {
            //m_TODO: LOG Failure At Cleanup
        }
    }

    //m_TODO: LOG Exiting App
    return main_return;

}

namespace application {
    char Init() {
        return 0;
    }

    MainStates Update() {
        return kMainUpdate;
    }

    char CleanUp() {
        return 0;
    }
} 