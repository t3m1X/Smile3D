#include "Libraries/SDL/include/SDL.h"
#include "Application/Application.h"
#include "Utilities/Logger.h"

#pragma comment ( lib, "Libraries/SDL/libx64/SDL2.lib" )
#pragma comment ( lib, "Libraries/SDL/libx64/SDL2main.lib" )

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

enum MainStates {
    kMainStart = 0,
    kMainUpdate,
    kMainFinish,
    kMainExit
};

int main(int argc, char** argv) {
    CONSOLE_LOG("Starting app...");

    int main_return = EXIT_FAILURE;
    MainStates app_state = kMainStart;

    // -- Application Initialize --
    SYSTEM_LOG("-------------- Application Init --------------");
    if (application::Init()) {
        CONSOLE_LOG("-------------- Application Update --------------");
        app_state = kMainUpdate;
    }
    else {
        SYSTEM_LOG("[Error] Application failed to Init properly");
        app_state = kMainExit;
    }

    while (app_state == kMainUpdate) {
        app_state = static_cast<MainStates>(1 + application::Update()); // app_state is always 1 in here
    }

    if (app_state == kMainFinish) {
        CONSOLE_LOG("-------------- Application CleanUp --------------");

        if (application::CleanUp())
            main_return = EXIT_SUCCESS;
        else
            CONSOLE_LOG("[Error] Application failed to CleanUp properly");

    }

    CONSOLE_LOG("Exiting app...");
    return main_return;

}