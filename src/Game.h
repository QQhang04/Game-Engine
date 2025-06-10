#ifndef GAME_H
#define GAME_H 

#include <SDL2/SDL.h>

class Game {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool isRunning;

    public:
        Game();
        ~Game();

        void Initialize();
        void Run();
        
        void ProcessInput();
        void Update();
        void Render();
       
       void Destroy();
        
};

#endif