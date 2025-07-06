#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Log/Logger.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

#include <SDL2/SDL.h>
#include <string>

class KeyboardControlSystem : public System {
    public:
        KeyboardControlSystem() {
            Logger::Log("KeyboardControlSystem initialized");
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->Subscribe<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
        }

        void OnKeyPressed(KeyPressedEvent& event) {
            std::string keyCode = std::to_string(event.symbol);
            std::string keySymbol(1, event.symbol);

            Logger::LogEvent("接收到Key pressed event: " + keyCode + " " + keySymbol);
        }
};

#endif
