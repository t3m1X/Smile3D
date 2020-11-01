#include "Application.h"
#include "../globals.hpp"
#include "../Utilities/Logger.h"

namespace application {

UpdateStatus ModulesPreUpdate() {
    return kUpdateContinue;
}
UpdateStatus ModulesUpdate() {
    return kUpdateContinue;
}
UpdateStatus ModulesPostUpdate() {
    return kUpdateContinue;
}    

char Init() {
    char ret = 0;

	logger::InitLogger(1000); //mTODO: This might get added to config

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
	return 0;
}

} //namespace application
