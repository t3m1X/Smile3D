#ifndef SMILE3D_APPLICATION_APPLICATION_H
#define SMILE3D_APPLICATION_APPLICATION_H
#include "globals.hpp"

namespace application {

char Init();
//Returns 0 on continue, 1 on exit and 2 on error
int Update();
char CleanUp();

} //namespace application


#endif