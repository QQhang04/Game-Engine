#ifndef GAME_H
#define GAME_H 

#include "../ECS/ECS.h"
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../AssetStore/AssetStore.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool isRunning;
        int windowWidth;
        int windowHeight;

        int millisecsPreviousFrame;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore;

    public:
        Game();
        ~Game();

        void Initialize();
        void Setup();
        
        void Run();
        
        void ProcessInput();
        void Update();
        void Render();
       
       void Destroy();
        
};

#endif