#ifndef SMILE3D_MODULES_MODINPUT_H
#define SMILE3D_MODULES_MODINPUT_H

#include "../globals.hpp"

namespace input {
    char Init();
    UpdateStatus PreUpdate();
    char CleanUp();

    //Checkers keyboard
    bool GetKey (int id);
    bool GetKeyDown (int id);
    bool GetKeyUp (int id);

    //Checkers mouse
    bool GetMouseButton (int id);
    bool GetMouseButtonDown (int id);
    bool GetMouseButtonUp (int id);

} //namespace input


#endif //SMILE3D_MODULES_MODINPUT_H