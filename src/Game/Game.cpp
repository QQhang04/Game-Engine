#include "Game.h"
#include "../Log/Logger.h"
#include "../ECS/ECS.h"
#include <iostream>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

#include "../MapLoader/MapLoader.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"

Game::Game() {
    Logger::Log("Game constructed");    
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
}

Game::~Game() {
    Logger::Log("Game destroyed");
}

void Game::Initialize() {
    Logger::Log("Game initialized");
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
        SDL_WINDOW_RESIZABLE
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

void Game::LoadLevel(int level = 1) {
    // load assets
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

    // add systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();    

    // load tilemap
    int tileScale = 2;
    std::vector<std::vector<int>> map = MapLoader::LoadMap("./assets/tilemaps/jungle.map");
    std::vector<Entity> tiles(map.size() * map[0].size());
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            Entity tile = registry->CreateEntity();
            int tileId = map[i][j];
            int row = tileId / 10, col = tileId % 10;

            int textureWidth, textureHeight;
            SDL_QueryTexture(assetStore->GetTexture("tilemap-image"), nullptr, nullptr, &textureWidth, &textureHeight);
            int tileSize = textureWidth / 10;

            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, col * tileSize, row * tileSize);
            tile.AddComponent<TransformComponent>(glm::vec2(j * tileSize * tileScale, i * tileSize * tileScale), glm::vec2(tileScale, tileScale), 0.0);
            tiles[i * map[i].size() + j] = tile;
        }
    }

    // add entities
    Entity tank = registry->CreateEntity();
    Entity truck = registry->CreateEntity();

    // add components
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(2.0, 2.0));
    tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);

    // truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 15.0));
    // truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
    // truck.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
}


void Game::Setup() {
    LoadLevel(1);
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

    // Invoke 所有System的Update
    registry->GetSystem<MovementSystem>().Update(deltaTime);

    // 等所有系统update完成，update Registry中的waiting list中要新加入的entity
    registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // Invoke所有要渲染的System的Update
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}