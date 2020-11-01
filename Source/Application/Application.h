#ifndef SMILE3D_APPLICATION_APPLICATION_H
#define SMILE3D_APPLICATION_APPLICATION_H
#include "../globals.hpp"

namespace application {

char Init(); //Application Initialization

UpdateStatus Update(); //Application Update

char CleanUp(); //Application Exit Cleanup

} //namespace application


#endif