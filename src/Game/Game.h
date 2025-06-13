#ifndef GAME_H
#define GAME_H 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

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