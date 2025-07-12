#ifndef KEYRELEASEDEVENT_H
#define KEYRELEASEDEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>

class KeyReleasedEvent: public Event {
    public:
        SDL_Keycode symbol;
        
        KeyReleasedEvent(SDL_Keycode symbol): symbol(symbol) {}
};

#endif 