#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Log/Logger.h"
#include "../EventBus/EventBus.h"

#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"

#include "../Components/CharacterControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL2/SDL.h>
#include <string>

class KeyboardControlSystem : public System {
    private:
        // 添加成员变量来跟踪按键状态
        bool isLeftPressed = false;
        bool isRightPressed = false;
        bool isUpPressed = false;
        bool isDownPressed = false;

        int latestDir = -1; // 0: up, 1: down, 2: left, 3: right

        glm::vec2 GetDirectionVector(int dir) {
            switch(dir) {
                case 0: return glm::vec2(0.0f, -1.0f); // 上
                case 1: return glm::vec2(0.0f, 1.0f);  // 下
                case 2: return glm::vec2(-1.0f, 0.0f); // 左
                case 3: return glm::vec2(1.0f, 0.0f);  // 右
                default: return glm::vec2(0.0f, 0.0f);
            }
        }

    public:
        KeyboardControlSystem() {
            RequireComponent<CharacterControlledComponent>();
            RequireComponent<RigidBodyComponent>();
            RequireComponent<SpriteComponent>();
            Logger::Log("KeyboardControlSystem initialized");
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->Subscribe<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
            eventBus->Subscribe<KeyReleasedEvent>(this, &KeyboardControlSystem::OnKeyReleased);
        }

        void OnKeyPressed(KeyPressedEvent& event) {
            std::string keyCode = std::to_string(event.symbol);
            std::string keySymbol(1, event.symbol);
            
            // 更新按键状态
            if (event.symbol == SDLK_LEFT || event.symbol == SDLK_a) {
                isLeftPressed = true;
                latestDir = 2;
            }
            if (event.symbol == SDLK_RIGHT || event.symbol == SDLK_d) {
                isRightPressed = true;
                latestDir = 3;
            }
            if (event.symbol == SDLK_UP || event.symbol == SDLK_w) {
                isUpPressed = true;
                latestDir = 0;
            }
            if (event.symbol == SDLK_DOWN || event.symbol == SDLK_s) {
                isDownPressed = true;
                latestDir = 1;
            }

            UpdateMovement();
        }

        void OnKeyReleased(KeyReleasedEvent& event) {
            // 更新按键状态
            if (event.symbol == SDLK_LEFT || event.symbol == SDLK_a) {
                isLeftPressed = false;
            }
            if (event.symbol == SDLK_RIGHT || event.symbol == SDLK_d) {
                isRightPressed = false;
            }
            if (event.symbol == SDLK_UP || event.symbol == SDLK_w) {
                isUpPressed = false;
            }
            if (event.symbol == SDLK_DOWN || event.symbol == SDLK_s) {
                isDownPressed = false;
            }

            UpdateMovement();
        }

    private:
        void UpdateMovement() {
            for (auto entity : GetSystemEntities()) {
                auto& characterControlled = entity.GetComponent<CharacterControlledComponent>();
                auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();

                bool isBothPressed = (isUpPressed + isDownPressed + isLeftPressed + isRightPressed) >= 2;
                bool isAnyPressed = isUpPressed || isDownPressed || isLeftPressed || isRightPressed;
                
                glm::vec2 finalDir = glm::vec2(0.0f, 0.0f);
                
                if (isBothPressed && latestDir != -1) {
                    // 当同时按下多个键时，使用最后按下的方向
                    finalDir = GetDirectionVector(latestDir);
                } else if (isAnyPressed) {
                    // 计算水平和垂直输入
                    float horizontalInput = 0.0f;
                    float verticalInput = 0.0f;
                    
                    // 检查水平输入
                    if (isLeftPressed) horizontalInput -= 1.0f;
                    if (isRightPressed) horizontalInput += 1.0f;
                    
                    // 检查垂直输入
                    if (isUpPressed) verticalInput -= 1.0f;
                    if (isDownPressed) verticalInput += 1.0f;

                    finalDir = glm::vec2(horizontalInput, verticalInput);
                }

                // 得到finalDir后更新刚体速度和动画sprite

                if (isAnyPressed) {
                    glm::vec2 direction = glm::normalize(finalDir);
                    rigidBody.velocity = direction * characterControlled.speed;
                } else {
                    rigidBody.velocity = glm::vec2(0.0f, 0.0f);
                }

                // 根据速度方向更新sprite的row和角色方向
                int colIndex = 0;
                if (isAnyPressed) {
                    if (finalDir.x != 0.0f) {
                        colIndex = finalDir.x > 0 ? 1 : 3;
                        characterControlled.currentDirection = finalDir.x > 0 ? Direction::RIGHT : Direction::LEFT;
                    } 
                    else if (finalDir.y != 0.0f) {
                        colIndex = finalDir.y > 0 ? 2 : 0;
                        characterControlled.currentDirection = finalDir.y > 0 ? Direction::DOWN : Direction::UP;
                    }

                    sprite.srcRect.y = sprite.height * colIndex;
                }
            }
        }
};

#endif
