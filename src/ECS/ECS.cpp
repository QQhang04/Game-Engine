#include "ECS.h"
#include "../Log/Logger.h"

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

template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
    TSystem* newSystem = new TSystem(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
    Logger::Log("Add System" + std::string(typeid(TSystem).name()));
}

template <typename TSystem>
void Registry::RemoveSystem() {
    systems.erase(std::type_index(typeid(TSystem)));
    Logger::Log("Remove System" + std::string(typeid(TSystem).name()));
}

template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
    return *(std::static_pointer_cast<TSystem>(systems[std::type_index(typeid(TSystem))]));
}


// Registry impl
void Registry::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entitySignature = entityComponentSignatures[entityId];

    for (auto& system : systems) {
        const auto& systemSignature = system.second -> GetComponentSignature();
        bool isInterested = (entitySignature & systemSignature) == systemSignature;
        if (isInterested) {
            system.second -> AddEntityToSystem(entity);
        }
    }
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