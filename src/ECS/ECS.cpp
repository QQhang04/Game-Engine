#include "ECS.h"

int IComponent::nextId = 0;

// Entity impl
int Entity::GetId() const {
    return id;
}

// System impl
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

// Registry impl
void Registry::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entitySignature = entityComponentSignatures[entityId];

    for (auto& system : systems) {
        const auto& systemSignature = system.second->GetComponentSignature();
        bool isInterested = (entitySignature & systemSignature) == systemSignature;
        if (isInterested) {
            system.second->AddEntityToSystem(entity);
        }
    }
}

Entity Registry::CreateEntity() {
    const int entityId = numEntities++;
    Entity entity(entityId);
    entity.registry = this;
    entitiesToAdd.insert(entity);

    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
    }

    Logger::Log("Entity created with id: " + std::to_string(entityId));
    return entity;
}

void Registry::Update() {
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