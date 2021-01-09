#include "Application.h"
#include "globals.hpp"
#include "Utilities/Logger.h"
#include "Libraries/SDL/include/SDL_keycode.h"

//Modules
#include "Modules/ModInput.h"
#include "Modules/ModWindow.h"
#include "Modules/ModTasker.h"
#include "Modules/ModFS.h"

namespace application {

UpdateStatus ModulesPreUpdate() {
    UpdateStatus ret = kUpdateContinue;

    ret = input::PreUpdate();

    return ret;
}
UpdateStatus ModulesUpdate() {
    if (input::GetKey(SDL_SCANCODE_ESCAPE))
        return kUpdateFinish; //mTODO: This should not go here in the future

    return kUpdateContinue;
}
UpdateStatus ModulesPostUpdate() {
    return kUpdateContinue;
}    

char Init() {
	logger::InitLogger(1000); //mTODO: This might get added to config

    char ret = tasker::Init(); 
    if (!ret)
        return ret;

    ret = filesys::Init(); 
    if (!ret)
        return ret;
        
    ret = input::Init();
    if (!ret)
        return ret;

    ret = window::Init();
    
	return ret;
}

int Update() {
    UpdateStatus return_status = ModulesPreUpdate();

    if (return_status == kUpdateContinue)
        return_status = ModulesUpdate();

    if (return_status == kUpdateContinue)
        return_status = ModulesPostUpdate();

	return static_cast<int>(return_status);
}

char CleanUp() {

    // We CleanUp in reverse order of initialization
	char ret = window::CleanUp();
    ret = input::CleanUp();
    ret = tasker::CleanUp();

    return ret;
}

} //namespace application
