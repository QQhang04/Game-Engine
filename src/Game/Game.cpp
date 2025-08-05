#include <iostream>

#include "Game.h"
#include "../Log/Logger.h"
#include "../ECS/ECS.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CharacterControlledComponent.h"
#include "../Components/TargetCameraComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"

#include "../MapLoader/MapLoader.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthBarSystem.h"
#include "../Systems/RenderGUISystem.h"

#include "../Systems/Debug/RenderBoxColliderSystem.h"

#include "../Events/CollisionEvent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth = 0;
int Game::mapHeight = 0;

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

    if (TTF_Init() != 0) {
        Logger::Err(std::string("TTF_Init Error: ") + TTF_GetError());
        return;
    }
    
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        Logger::Err("Error creating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    // Initialize the ImGui context
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

    // Initialize the camera view with the entire screen area
    camera = {0, 0, windowWidth, windowHeight};

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    
    isRunning = true;

}

void Game::LoadLevel(int level = 1) {
    // load assets
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
    assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
    assetStore->AddTexture(renderer, "bullet-texture", "./assets/images/bullet.png");
    assetStore->AddTexture(renderer, "tree-image", "./assets/images/tree.png");
    assetStore->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 16);
    assetStore->AddFont("health-bar-font", "./assets/fonts/charriot.ttf", 12);
    assetStore->AddFont("pico8-font-5", "./assets/fonts/charriot.ttf", 12);
    // add systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();    
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<RenderBoxColliderSystem>();
    registry->AddSystem<ProjectileSystem>();
    registry->AddSystem<ProjectileLifecycleSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderHealthBarSystem>();
    registry->AddSystem<RenderGUISystem>();
    // Subscribe to events (只需要订阅一次)
    registry->GetSystem<RenderBoxColliderSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);

    // load tilemap
    float tileScale = 2.5;
    std::vector<std::vector<int>> map = MapLoader::LoadMap("./assets/tilemaps/jungle.map");
    std::vector<Entity> tiles(map.size() * map[0].size());
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            Entity tile = registry->CreateEntity();
            int tileId = map[i][j];
            int row = tileId / 10, col = tileId % 10;

            int textureWidth, textureHeight;
            SDL_QueryTexture(assetStore->GetTexture("tilemap-image"), nullptr, nullptr, &textureWidth, &textureHeight);
            int tileSize = 32;

            tile.AddComponent<SpriteComponent>("tilemap-image", 32, 32, 0, col * 32, row * 32);
            tile.AddComponent<TransformComponent>(glm::vec2(j * tileSize * tileScale, i * tileSize * tileScale), glm::vec2(tileScale, tileScale), 0.0);
            tiles[i * map[i].size() + j] = tile;
        }
    }
    mapWidth = map[0].size() * 32 * tileScale;
    mapHeight = map.size() * 32 * tileScale;

    // add entities
    // TODO 完成一个可以切换控制角色的系统
    Entity chopper = registry->CreateEntity();
    Entity tank = registry->CreateEntity();
    Entity truck = registry->CreateEntity();
    Entity radar = registry->CreateEntity();

    // TODO 使用随机数算法随机在草地上生成树木
    Entity tree1 = registry->CreateEntity();
    Entity tree2 = registry->CreateEntity();
    

    Entity label = registry->CreateEntity();
    // 放在屏幕中间偏上的位置
    label.AddComponent<TextLabelComponent>(glm::vec2(windowWidth / 2.0f - 40.0f, 50.0f), "Hello World", "charriot-font", SDL_Color{255, 255, 255, 255}, true);
    
    // add tags or groups
    chopper.AddTag("player");
    tree1.AddToGroup("obstacle");
    tree2.AddToGroup("obstacle");
    tank.AddToGroup("enemy");
    truck.AddToGroup("enemy");

    // add components
    tank.AddComponent<TransformComponent>(glm::vec2(750.0, 698.0), glm::vec2(2.0, 2.0));
    tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1, 0, 0);
    tank.AddComponent<BoxColliderComponent>(glm::vec2(32.0, 32.0), glm::vec2(0.0, 0.0));
    // tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(50.0, 0.0), 1000, 1000, 10, true);
    tank.AddComponent<HealthComponent>(100);

    chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(2.0, 2.0));
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 2, 0, 0);
    chopper.AddComponent<AnimationComponent>(2, 15);
    chopper.AddComponent<CharacterControlledComponent>(100.0f);
    chopper.AddComponent<TargetCameraComponent>();
    chopper.AddComponent<HealthComponent>(100);
    chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(400.0, 0.0), 100, 5000, 10, true);

    radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 100, 10.0), glm::vec2(1.0, 1.0));
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 3, 0, 0, true);
    radar.AddComponent<AnimationComponent>(8, 1);

    truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
    truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(2.0, 2.0), 0.0);
    truck.AddComponent<BoxColliderComponent>(glm::vec2(32.0, 32.0), glm::vec2(0.0, 0.0));
    truck.AddComponent<HealthComponent>(100);

    tree1.AddComponent<TransformComponent>(glm::vec2(650, 698), glm::vec2(2, 2));
    tree1.AddComponent<SpriteComponent>("tree-image", 32, 32, 2);
    tree1.AddComponent<BoxColliderComponent>(glm::vec2(25, 20), glm::vec2(5, 5));
    tree1.AddComponent<HealthComponent>(50);

    tree2.AddComponent<TransformComponent>(glm::vec2(850, 698), glm::vec2(2, 2));
    tree2.AddComponent<SpriteComponent>("tree-image", 32, 32, 2);
    tree2.AddComponent<BoxColliderComponent>(glm::vec2(25, 20), glm::vec2(5, 5));
    tree2.AddComponent<HealthComponent>(50);
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
        // 处理ImGui SDL event
        ImGui_ImplSDL2_ProcessEvent(&event);
        ImGuiIO& io = ImGui::GetIO();
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        // 处理SDL event
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                if (event.key.keysym.sym == SDLK_b) {
                    isDebugMode = !isDebugMode;
                }
                eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                eventBus->EmitEvent<KeyReleasedEvent>(event.key.keysym.sym);
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
    registry->GetSystem<ProjectileLifecycleSystem>().Update(registry);
    registry->GetSystem<CollisionSystem>().Update(eventBus); // CollisionSystem会发起事件
    registry->GetSystem<ProjectileSystem>().Update(registry);
    registry->GetSystem<CameraMovementSystem>().Update(camera);

    // 等所有系统update完成，update Registry中的waiting list中要新加入的entity
    registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // Invoke所有要渲染的System的Update
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore);
    registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetStore, camera);
    // 如果debug模式开启，则Invoke所有需要渲染的Debug System的Update
    if (isDebugMode) {
        registry->GetSystem<RenderBoxColliderSystem>().Update(renderer, camera);
        registry->GetSystem<RenderGUISystem>().Update(registry, camera);
    }

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}