#include "Game.h"
#include "../Log/Logger.h"
#include "../ECS/ECS.h"
#include <iostream>

Game::Game() {
    Logger::Log("Game initialized");
    isRunning = false;
}

Game::~Game() {
    Logger::Log("Game destroyed");
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err(std::string("SDL_Init Error: ") + SDL_GetError());
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800;//displayMode.w;
    windowHeight = 600;//displayMode.h;

    window = SDL_CreateWindow(
        "QQh Game", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        windowWidth, 
        windowHeight, 
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        Logger::Err(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Logger::Err(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
        return;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    isRunning = true;

}


void Game::Setup() {
    // TODO : Setup the game
    // Entity tank = registry.CreateEntity();
    // tank.AddComponent<TransformComponent>(glm::vec2(20, 20));
    // tank.AddComponent<BoxColliderComponent>(glm::vec2(10, 15));
    // tank.AddComponent<SpriteComponent>("./assets/images/tank-tiger-right.png");
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                break;
        }
    }
}

void Game::Update() {
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) { // 防止程序运行太慢
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();

    // TODO
    // MovementSystem.Update();
    // CollisionSystem.Update();
    // DamageSystem.Update();

}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);


    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}