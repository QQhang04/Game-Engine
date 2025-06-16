# Day 8 Start A First Entity & Smart Pointers



#### 创建Entity流程：

1.Game::Setup()

```cpp
void Game::Setup() {
    // TODO : Setup the game
    Entity tank = registry->CreateEntity();
    Entity truck = registry->CreateEntity();
}
```

2.

```cpp
Entity Registry::CreateEntity() {
    const int entityId = numEntities++;
    Entity entity(entityId);
    entitiesToAdd.insert(entity);

    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
    }

    Logger::Log("Entity created with id: " + std::to_string(entityId));
    return entity;
}
```

3.下一帧统一Add

```cpp
void Registry::Update() {
    // TODO
    // add the entities in the waiting list
    for (auto entity : entitiesToAdd) {
        AddEntityToSystems(entity);
    }
    entitiesToAdd.clear();

    for (auto entity : entitiesToRemove) {
        // RemoveEntityFromSystems(entity); // TODO remove the entities in the waiting list
    }
    entitiesToRemove.clear();

}
```



#### Smart Pointer智能指针

#include <memory>

手动的delete new出来的内存 —> 使用智能指针

##### 1.Unique Pointer

只有一个owner，owner出作用域时自动释放内存

使用`make_unique<T>()`

```cpp
str::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
```



##### 2.Shared Pointer

使用`make_shared<T>()`

拥有counter记录owner，可以有多个owner，只会在最后一个owner出作用域时释放内存

```cpp
str::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
```

![image-20250615205747589](/Users/qqhang/Library/Application Support/typora-user-images/image-20250615205747589.png)



注意：SDL raw pointer 不要用智能指针封装
