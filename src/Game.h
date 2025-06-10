#ifndef GAME_H
#define GAME_H 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool isRunning;
        int windowWidth;
        int windowHeight;

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