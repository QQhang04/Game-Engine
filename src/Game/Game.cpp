#include <iostream>

#include "Game.h"
#include "../Log/Logger.h"
#include "../ECS/ECS.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"

#include "../MapLoader/MapLoader.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/KeyboardMovementSystem.h"

#include "../Systems/Debug/RenderBoxColliderSystem.h"

#include "../Events/CollisionEvent.h"
#include "../Events/KeyPressedEvent.h"


Game::Game() {
    Logger::Log("Game constructed");    
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
}

Game::~Game() {
    Logger::Log("Game destroyed");
}

void Game::Initialize() {
    Logger::Log("Game initialized");
    isDebugMode = false;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err(std::string("SDL_Init Error: ") + SDL_GetError());
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    
    // 设置游戏逻辑分辨率
    windowWidth = 800;
    windowHeight = 640;

    window = SDL_CreateWindow(
        "QQh Game", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        displayMode.w, 
        displayMode.h, 
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
    
    // 设置逻辑渲染尺寸为800x600，SDL会自动缩放到实际屏幕尺寸
    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

    isRunning = true;

}

void Game::LoadLevel(int level = 1) {
    // load assets
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
    assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");

    // add systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();    
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<KeyboardControlSystem>();

    registry->AddSystem<RenderBoxColliderSystem>();

    // Subscribe to events (只需要订阅一次)
    registry->GetSystem<RenderBoxColliderSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    
    Logger::Log("Systems added");

    // load tilemap
    int tileScale = 1;
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

            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, col * tileSize, row * tileSize);
            tile.AddComponent<TransformComponent>(glm::vec2(j * tileSize * tileScale, i * tileSize * tileScale), glm::vec2(tileScale, tileScale), 0.0);
            tiles[i * map[i].size() + j] = tile;
        }
    }

    // add entities
    Entity chopper = registry->CreateEntity();
    Entity tank = registry->CreateEntity();
    Entity truck = registry->CreateEntity();
    Entity radar = registry->CreateEntity();

    // add components
    tank.AddComponent<TransformComponent>(glm::vec2(100.0, 10.0), glm::vec2(1.0, 1.0));
    tank.AddComponent<RigidBodyComponent>(glm::vec2(-15.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1, 0, 0);
    tank.AddComponent<BoxColliderComponent>(glm::vec2(32.0, 32.0), glm::vec2(0.0, 0.0));

    chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0));
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 2, 0, 0);
    chopper.AddComponent<AnimationComponent>(2, 15);

    radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 100, 10.0), glm::vec2(1.0, 1.0));
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 3);
    radar.AddComponent<AnimationComponent>(8, 1);

    truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
    truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<BoxColliderComponent>(glm::vec2(32.0, 32.0), glm::vec2(0.0, 0.0));
}


void Game::Setup() {
    LoadLevel(1);
}

void Game::Run() {
    Setup();
    while (isRunning) {
        HandleInput();
        Update();
        Render();
    }
}

void Game::HandleInput() {
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
                if (event.key.keysym.sym == SDLK_d) {
                    isDebugMode = !isDebugMode;
                }
                eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
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

    // Invoke System的逻辑Update
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();

    registry->GetSystem<CollisionSystem>().Update(eventBus); // CollisionSystem会发起事件

    // 等所有系统update完成，update Registry中的waiting list中要新加入的entity
    registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // Invoke所有要渲染的System的Update
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

    // 如果debug模式开启，则Invoke所有需要渲染的Debug System的Update
    if (isDebugMode) {
        registry->GetSystem<RenderBoxColliderSystem>().Update(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}