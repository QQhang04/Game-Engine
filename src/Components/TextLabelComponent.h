#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

struct TextLabelComponent {
    glm::vec2 position;
    std::string text;
    std::string assetId;
    SDL_Color color;
    bool isFixed;
    SDL_Texture* texture = nullptr; // 新增缓存纹理
    bool dirty = true; // 新增脏标记

    TextLabelComponent(glm::vec2 position = glm::vec2(0), const std::string& text = "", const std::string& assetId = "", const SDL_Color& color = {0, 0, 0}, bool isFixed = true) {
        this->position = position;
        this->text = text;
        this->assetId = assetId;
        this->color = color;
        this->isFixed = isFixed;
        this->texture = nullptr; // 初始化
        this->dirty = true; // 初始化
    }   

    void OnRemove() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
};

#endif
