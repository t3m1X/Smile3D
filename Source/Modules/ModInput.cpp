#include "ModInput.h"
#include <cstring>

#include "Utilities/Logger.h"
#include "Libraries/SDL/include/SDL.h"
#include "globals.hpp"

//Modules
#include "ModWindow.h"

namespace input {
enum KeyState {
	kKeyInvalid = -1,
	kKeyDown,
	kKeyRepeat,
	kKeyUp,
    kKeyIdle,
    kKeyDownDouble
};

const unsigned int kMaxKeys = 300;
const unsigned int kMaxMouseButtons = 5;
const unsigned int kMaxKeyEvents = 50;

unsigned int key_events_it = 0;
KeyState* key_events[kMaxKeyEvents];

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

    // Update keyboard and mouse states
    for (uint i = 0; i < key_events_it; ++i)
    {
        *key_events[i] = static_cast<KeyState>(static_cast<int>(*key_events[i]) + 1);
        key_events[i] = nullptr;
    }

    key_events_it = 0;

    SDL_Event e;
	while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
            ret = kUpdateFinish;
            break;

            case SDL_KEYUP:
            if (!e.key.repeat) {
                keyboard[e.key.keysym.scancode] = kKeyUp;
                key_events[key_events_it++] = &keyboard[e.key.keysym.scancode];
            }
            break;

            case SDL_KEYDOWN:
            if (!e.key.repeat) {
                keyboard[e.key.keysym.scancode] = kKeyDown;
                key_events[key_events_it++] = &keyboard[e.key.keysym.scancode];
            }
       
            break;

            case SDL_MOUSEBUTTONDOWN:
            if (e.button.clicks == 2)
                mouse_buttons[e.button.button] = kKeyDownDouble;
            else {
                mouse_buttons[e.button.button] = kKeyDown;
                key_events[key_events_it++] = &mouse_buttons[e.button.button];

            }
            break;

            case SDL_MOUSEBUTTONUP:
            mouse_buttons[e.button.button] = kKeyUp;
            key_events[key_events_it++] = &mouse_buttons[e.button.button];
            break;

            case SDL_WINDOWEVENT:

            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                window::SetSize(e.window.data1, e.window.data2);
            }
            
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