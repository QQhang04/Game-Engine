#ifndef GAME_H
#define GAME_H 

#include "../ECS/ECS.h"
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect camera;

        bool isRunning;
        bool isDebugMode;

        int millisecsPreviousFrame;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore;
        std::unique_ptr<EventBus> eventBus;

    public:
        Game();
        ~Game();

        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;

        void Initialize();
        void LoadLevel(int level);
        void Setup();
        
        void Run();
        
        void HandleInput();
        void Update();
        void Render();
       
       void Destroy();
        
};

#endif