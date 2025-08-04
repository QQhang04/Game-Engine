#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H

#include "../ECS/ECS.h"
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>

class RenderGUISystem : public System {
    public:
       RenderGUISystem() = default;
       void Update(const std::unique_ptr<Registry>& registry, const SDL_Rect& camera) {
            ImGui::NewFrame();
            
            // Spawn Enemy窗口
            if (ImGui::Begin("Spawn enemies")) {
                ImGui::Text("here is what we spawn new enemies");
                static int enemyXPos = 0;
                static int enemyYPos = 0;
                static int scaleX = 1;
                static int scaleY = 1;
                static int velX = 0;
                static int velY = 0;
                static int health = 100;
                static float rotation = 0.0;
                static float projAngle = 0.0;
                static float projSpeed = 100.0;
                static int projRepeat = 10;
                static int projDuration = 10;
                const char* sprites[] = {"tank-image", "truck-image"};
                static int selectedSpriteIndex = 0;

                if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("position x", &enemyXPos);
                    ImGui::InputInt("position y", &enemyYPos);
                    ImGui::SliderInt("scale x", &scaleX, 1, 10);
                    ImGui::SliderInt("scale y", &scaleY, 1, 10);
                    ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("velocity x", &velX);
                    ImGui::InputInt("velocity y", &velY);
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
                    ImGui::SliderFloat("speed (px/sec)", &projSpeed, 10, 500);
                    ImGui::InputInt("repeat (sec)", &projRepeat);
                    ImGui::InputInt("duration (sec)", &projDuration);
                }
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderInt("%", &health, 0, 100);
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                if (ImGui::Button("Spawn enemy")) {
                    Entity enemy = registry->CreateEntity();
                    enemy.AddToGroup("enemy");
                    enemy.AddComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                    enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
                    enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 2);
                    enemy.AddComponent<BoxColliderComponent>(glm::vec2(25, 20), glm::vec2(5, 5));
                    double projVelX = cos(projAngle) * projSpeed; 
                    double projVelY = sin(projAngle) * projSpeed;
                    enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), projRepeat * 1000, projDuration * 1000, 10, false);
                    enemy.AddComponent<HealthComponent>(health);
                }
            }
            ImGui::End();

            // 显示鼠标坐标窗口
            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.9f);
            if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
                ImGui::Text(
                    "Map coordinates (x=%.1f, y=%.1f)",
                    ImGui::GetIO().MousePos.x + camera.x,
                    ImGui::GetIO().MousePos.y + camera.y
                );
            }
            ImGui::End();

            ImGui::Render();
            ImGuiSDL::Render(ImGui::GetDrawData());
       }
};

#endif