#ifndef SMILE3D_MODULES_MODWINDOW_H
#define SMILE3D_MODULES_MODWINDOW_H

#include "../globals.hpp"

class SDL_Window;
class SDL_Surface;

namespace window {

char Init();
char CleanUp();

void UpdateSize(); //To be called on resize

// --- Setters ---
void SetWidth(uint width);
void SetHeight(uint height);
void SetSize(uint width, uint height);
/* To be implemented 
void SetFullscreen(bool value);
void SetResizable(bool value);
void SetBorderless(bool value);
void SetFullscreenDesktop(bool value);
void SetBrightness(float value);
void SetTitle(const char* title);
void SetMouseFocus(bool focus);
*/

// --- Getters ---
uint GetWidth();
uint GetHeight();
/* To be implemented
float GetBrightness();
uint GetDisplayRefreshRate();
bool IsFullscreen();
bool IsResizable();
bool IsBorderless();
bool IsFullscreenDesktop();
bool IsMouseFocused(); */

static SDL_Window* window; //The window we'll be rendering to

static SDL_Surface* screen_surface; //The surface contained by the window

} //namespace window


 #endif //SMILE3D_MODULES_MODWINDOW_H