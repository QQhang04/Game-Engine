#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include "../Log/Logger.h"
#include <memory>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
    protected:
        static int nextId;
};

template<typename TComponent>
class Component : public IComponent {
    public:
        static int GetId() {
            static auto id = nextId++;
            return id;
        }
};

class Entity {
    private:
        int id;

    public:
        Entity(int id) : id(id) {}
        Entity(const Entity& other) = default;
        int GetId() const;

        Entity& operator=(const Entity& other) = default;
        bool operator<(const Entity& other) const { return id < other.id;}
        bool operator==(const Entity& other) const { return id == other.id;}
        bool operator!=(const Entity& other) const { return id != other.id;}

        template<typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template<typename TComponent> void RemoveComponent();
        template<typename TComponent> bool HasComponent() const;
        template<typename TComponent> TComponent& GetComponent() const;

        class Registry* registry; // forward declaration(modern cpp syntax)
};

class System {
    private:
        Signature componentSignature;
        std::vector<Entity> entities;

    public:
        System() = default;
        ~System() = default;

        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        template<typename TComponent> void RequireComponent();
};

class IPool {
    public:
        virtual ~IPool() {};
};

// is a vector(continous data) of objects of type T
template<typename T>
class Pool : public IPool {
    private:
        std::vector<T> data;

    public:
        Pool(int size = 100) {
            data.resize(size);
        }
        virtual ~Pool() = default;

        bool isEmpty() const {
            return data.empty();
        }

        int GetSize() const {
            return data.size();
        }

        void Resize(int size) {
            data.resize(size);
        }

        void Clear() {
            data.clear();
        }

        void Add(T object) {
            data.push_back(object);
        }

        T& operator[](int index) {
            return data[index];
        }

        void Set(int index, T object) {
            data[index] = object;
        }

        T& Get(int index) {
            return static_cast<T&>(data[index]);
        }


};

// the Entity Manager that coordinates everything
class Registry {
    private:
        int numEntities = 0;

        std::vector<std::shared_ptr<IPool>> componentPools; 

        std::vector<Signature> entityComponentSignatures;

        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

        // entities that are waited to be added / deleted
        std::set<Entity> entitiesToAdd;
        std::set<Entity> entitiesToRemove;

    public:
        Registry() {
            Logger::Log("Registry constructed");
        }
        ~Registry() {
            Logger::Log("Registry destroyed");
        }

        void Update();

        // create a new entity
        Entity CreateEntity();
        
        // Component Management
        template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
        template <typename TComponent> void RemoveComponent(Entity entity);
        template <typename TComponent> bool HasComponent(Entity entity) const;
        template <typename TComponent> TComponent& GetComponent(Entity entity) const;

        //System Management
        template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template <typename TSystem> void RemoveSystem();
        template <typename TSystem> bool HasSystem() const;
        template <typename TSystem> TSystem& GetSystem() const;

        // only the entity can link to this system
        void AddEntityToSystems(Entity entity);

        // kill entity

        // add a component to entity: AddComponent<T>();

        // remove a component from entity
        // does a entity has a component

        // add system
        // remove system
        // has system
        // get system
};

// SYSTEM IMPL
template<typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

// REGISTRY Component manage IMPL
template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentPools.size() <= componentId) {
        componentPools.resize(componentId + 1);
    }

    if (componentPools[componentId] == nullptr) {
        componentPools[componentId] = std::make_shared<Pool<TComponent>>();
    }

    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityId, newComponent);

    if (entityComponentSignatures.size() <= entityId) {
        entityComponentSignatures.resize(numEntities);
    }
    entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Add Component " + std::string(typeid(TComponent).name()) + "id = " + std::to_string(componentId) + " to Entity id = " + std::to_string(entityId));
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

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
    const auto entityId = entity.GetId();
    const auto componentId = Component<TComponent>::GetId();

    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entityId);
}

// REGISTRY System manage IMPL
template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
    Logger::Log("Add System " + std::string(typeid(TSystem).name()));
}

template <typename TSystem>
void Registry::RemoveSystem() {
    systems.erase(std::type_index(typeid(TSystem)));
    Logger::Log("Remove System " + std::string(typeid(TSystem).name()));
}

template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}


// ENTITY IMPL
template<typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void Entity::RemoveComponent() {
    registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::HasComponent() const {
    return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent& Entity::GetComponent() const {
    return registry->GetComponent<TComponent>(*this);
}

#endif