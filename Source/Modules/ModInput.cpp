#include "ModInput.h"
#include <cstring>

#include "../Utilities/Logger.h"
#include "../Libraries/SDL/include/SDL.h"
#include "../globals.hpp"


namespace input {
enum KeyState {
	kKeyIdle = 0,
	kKeyDown,
	kKeyRepeat,
	kKeyUp,
    kKeyDownDouble
};

const unsigned int kMaxKeys = 300;
const unsigned int kMaxMouseButtons = 5;
KeyState* keyboard;
KeyState* mouse_buttons;

char Init() {
    char ret = 1;

    keyboard = new KeyState[kMaxKeys];
    memset(keyboard, kKeyIdle, sizeof(KeyState) * kMaxKeys);

    mouse_buttons = new KeyState[kMaxMouseButtons];
    memset(mouse_buttons, kKeyIdle, sizeof(KeyState) * kMaxMouseButtons);

    // -- Initialize SDL --
    CONSOLE_LOG("Initializing SDL input event system");
    SDL_Init(0);

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
		CONSOLE_LOG("[Error]: SDL_EVENTS could not initialized! SDL_Error: %s\n", SDL_GetError());
		ret = 0;
	}

    return ret;
}

UpdateStatus PreUpdate() {
    UpdateStatus ret = kUpdateContinue;
    SDL_PumpEvents();
	//const Uint8* keys = SDL_GetKeyboardState(NULL);

    // Update keyboard states
    for (int i = 0; i < kMaxKeys; ++i) {
        if (keyboard[i] == kKeyDown)
            keyboard[i] = kKeyRepeat;
        else if (keyboard[i] == kKeyUp)
            keyboard[i] = kKeyIdle;
    }

    //Update mouse states
    for (int i = 0; i < kMaxMouseButtons; ++i) {
        if (mouse_buttons[i] == kKeyDown || mouse_buttons[i] == kKeyDownDouble)
            mouse_buttons[i] = kKeyRepeat;
        else if (mouse_buttons[i] == kKeyUp)
            mouse_buttons[i] = kKeyIdle;
    }

    SDL_Event e;
	while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
            ret = kUpdateFinish;
            break;

            case SDL_KEYUP:
            if (!e.key.repeat)
                keyboard[e.key.keysym.scancode] = kKeyUp;
            break;

            case SDL_KEYDOWN:
            if (!e.key.repeat)
                keyboard[e.key.keysym.scancode] = kKeyDown;
            break;

            case SDL_MOUSEBUTTONDOWN:
            if (e.button.clicks == 2)
                mouse_buttons[e.button.button] = kKeyDownDouble;
            else
                mouse_buttons[e.button.button] = kKeyDown;
            break;

            case SDL_MOUSEBUTTONUP:
                mouse_buttons[e.button.button] = kKeyUp;
            break;

        }

    }


    return ret;
}

char CleanUp() {
    delete[] keyboard;
    delete[] mouse_buttons;
    keyboard = mouse_buttons = nullptr;

    CONSOLE_LOG("Quitting SDL input event system");
    SDL_QuitSubSystem(SDL_INIT_EVENTS);

    return 1;
}

bool GetKey(int id) {
    const KeyState* m_keyboard = keyboard;
    return m_keyboard[id] == kKeyDown || m_keyboard[id] == kKeyRepeat;
}

bool GetKeyDown(int id) {
    const KeyState* m_keyboard = keyboard;
    return m_keyboard[id] == kKeyDown;
}

bool GetKeyUp(int id) {
    const KeyState* m_keyboard = keyboard;
    return m_keyboard[id] == kKeyUp;
}

bool GetMouseButton(int id) {
    const KeyState* m_mouse = mouse_buttons;
    return m_mouse[id] == kKeyDown || m_mouse[id] == kKeyRepeat;
}

bool GetMouseButtonDown(int id) {
    const KeyState* m_mouse = mouse_buttons;
    return m_mouse[id] == kKeyDown;
}

bool GetMouseButtonUp(int id) {
    const KeyState* m_mouse = mouse_buttons;
    return m_mouse[id] == kKeyUp;
}

} //namespace input