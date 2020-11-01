#include "Application.h"
#include "../globals.hpp"

namespace application {

char Init() {
	return 0;
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

UpdateStatus ModulesPreUpdate() {
    return kUpdateContinue;
}
UpdateStatus ModulesUpdate() {
    return kUpdateContinue;
}
UpdateStatus ModulesPostUpdate() {
    return kUpdateContinue;
}

} //namespace application
