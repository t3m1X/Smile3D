#include "Application.h"
#include "../globals.hpp"
#include "../Utilities/Logger.h"
#include "../Modules/ModInput.h"
#include "../Libraries/SDL/include/SDL_keycode.h"

namespace application {

UpdateStatus ModulesPreUpdate() {
    UpdateStatus ret = kUpdateContinue;

    ret = input::PreUpdate();

    return ret;
}
UpdateStatus ModulesUpdate() {
    if (input::GetKey(SDL_SCANCODE_ESCAPE))
        return kUpdateFinish;

    return kUpdateContinue;
}
UpdateStatus ModulesPostUpdate() {
    return kUpdateContinue;
}    

char Init() {
    char ret = 1;

	logger::InitLogger(1000); //mTODO: This might get added to config

    ret = input::Init();
    if (!ret)
        return ret;

    
    
	return ret;
}

UpdateStatus Update() {
    UpdateStatus return_status = kUpdateContinue;

    return_status = ModulesPreUpdate();

    if (return_status == kUpdateContinue)
        return_status = ModulesUpdate();

    if (return_status == kUpdateContinue)
        return_status = ModulesPostUpdate();

	return return_status;
}

char CleanUp() {
	char ret = 1;

    ret = input::CleanUp();

    return ret;
}

} //namespace application
