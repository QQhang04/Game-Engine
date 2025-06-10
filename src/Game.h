#ifndef GAME_H
#define GAME_H

class Game {
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