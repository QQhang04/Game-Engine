#include "ECS.h"
#include "../Log/Logger.h"

int IComponent::nextId = 0;

int Entity::GetId() const {
    return id;
}

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {return entity == other;}), 
        entities.end()
    );
}

std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity() {
    const int entityId = numEntities++;
    Entity entity(entityId);
    entitiesToAdd.insert(entity);

    Logger::Log("Entity created with id: " + std::to_string(entityId));
    return entity;
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentPools.size() <= componentId) {
        componentPools.resize(componentId + 1);
    }

    if (componentPools[componentId] == nullptr) {
        componentPools[componentId] = new Pool<TComponent>();
    }

    Pool<TComponent>* componentPool = static_cast<Pool<TComponent>*>(componentPools[componentId]);

    if (entityId >= componentPool -> GetSize()) {
        componentPool -> Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...); 
    componentPool -> Set(entityId, newComponent);


    if (entityComponentSignatures.size() <= entityId) {
        entityComponentSignatures.resize(numEntities);
    }
    entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Add Component to Entity");
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
    const auto entityId = entity.GetId();
    const auto componentId = Component<TComponent>::GetId();

    if (entityComponentSignatures[entityId].test(componentId)) {
        entityComponentSignatures[entityId].reset(componentId);
    }
    Logger::Log("Remove Component from Entity");
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
    const auto entityId = entity.GetId();
    const auto componentId = Component<TComponent>::GetId();

    if (entityComponentSignatures[entityId].test(componentId)) {
        return true;
    }
    return false;
}   


void Registry::Update() {
    // TODO
    // add/remove the entities in the waiting list
}