#include "ModWindow.h"
#include "Utilities/Logger.h"
#include "Libraries/SDL/include/SDL.h"
#include "globals.hpp"

namespace window {

// --- Display ---
uint        refresh_rate = 0;
uint        screen_width = 1280;
uint        screen_height = 1024;
uint        display_width = 0; // To keep the original value
uint        display_height = 0; // To keep the original value
const uint  kMinHeight = 320;
const uint  kMinWidth = 950;

// --- Win Flags ---
bool    fullscreen = false;
bool    resizable = true;
bool    borderless = false;
bool    fullscreen_desktop = false;
bool    mouse_focus = false;

char Init() {
    CONSOLE_LOG("Init SDL window & surface");
	char ret = 1;

    window = nullptr;
    screen_surface = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		CONSOLE_LOG("[error]: SDL_INIT_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		CONSOLE_LOG("SDL_INIT_VIDEO initialized successfully");

		// --- Get Display Data ---
		SDL_DisplayMode display;
		SDL_GetCurrentDisplayMode(0, &display);
		display_width = display.w;
		display_height = display.h;

		// --- Assign Display Specific values to code vars ---
		screen_width = uint(display.w * 0.75f);
		screen_height = uint(display.h * 0.75f);

		refresh_rate = display.refresh_rate;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY); // deprecated functions are enabled --> SDL_GL_CONTEXT_PROFILE_CORE to disable || SDL_GL_CONTEXT_PROFILE_COMPATIBILITY to enable
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		if (fullscreen) { //mTODO: This should probably be read from a config somewhere
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (resizable) {
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (borderless) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (fullscreen_desktop ) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}


		window = SDL_CreateWindow("Smile3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);

		if (window == NULL) {
			CONSOLE_LOG("[error]: Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else {
			CONSOLE_LOG("Successfully created Window: %s", "Smile3D Engine");
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

char CleanUp() {
    CONSOLE_LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL) {
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return 1;
}

void UpdateSize() {
    SDL_SetWindowSize(window, screen_width, screen_height);
}

void SetHeight(uint height) {
    if (height < kMinHeight)
        screen_height = kMinHeight;
    else
        screen_height = height;

    UpdateSize();
}

void SetWidth(uint width) {
    if (width < kMinWidth)
        screen_width = kMinWidth;
    else
        screen_width = width;

    UpdateSize();
}

void SetSize(uint width, uint height) {
    if (width < kMinWidth)
        screen_width = kMinWidth;
    else
        screen_width = width;

    if (height < kMinHeight)
        screen_height = kMinHeight;
    else
        screen_height = height;

    UpdateSize();
}

uint GetWidth() {
	return screen_width;
}

uint GetHeight() {
	return screen_height;
}

} //namespace window