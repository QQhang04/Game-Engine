#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <deque>
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
        Entity(int id = 0) : id(id) {}
        Entity(const Entity& other) = default;
        int GetId() const;
        void Destroy() const;

        Entity& operator=(const Entity& other) = default;
        bool operator<(const Entity& other) const { return id < other.id;}
        bool operator==(const Entity& other) const { return id == other.id;}
        bool operator!=(const Entity& other) const { return id != other.id;}

        template<typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template<typename TComponent> void RemoveComponent();
        template<typename TComponent> bool HasComponent() const;
        template<typename TComponent> TComponent& GetComponent() const;

        void AddTag(const std::string& tag);
        bool RemoveTag();
        bool HasTag(const std::string& tag) const;
        std::string GetTag() const;

        void AddToGroup(const std::string& group);
        bool RemoveFromGroup();
        bool BelongToGroup(const std::string& group) const;
        std::string GetGroup() const;

        class Registry* registry;
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
        virtual void RemoveEntityComponent(int entityId) = 0;
};

// is a vector(continous data) of objects of type T
template<typename T>
class Pool : public IPool {
    private:
        std::vector<T> data;
        std::unordered_map<int, int> entityIdToPoolIndex;
        std::unordered_map<int, int> poolIndexToEntityId;
        int size;

    public:
        Pool(int size = 100) {
            data.resize(size);
            size = 0;
        }
        virtual ~Pool() = default;

        bool isEmpty() const {
            return data.empty();
        }

        int GetSize() const {
            return size;
        }

        void Resize(int size) {
            data.resize(size);
        }

        void Clear() {
            data.clear();
            size = 0;
        }

        void Add(T object) {
            data.push_basck(object);
        }

        T& operator[](int index) {
            return data[index];
        }

        void Set(int entityId, T object) {
            entityIdToPoolIndex[entityId] = size;
            poolIndexToEntityId[size] = entityId;
            if (size + 1 >= data.size() - 1) {
                data.resize(data.size() * 2);
            }
            data[size++] = object;
        }

        T& Get(int entityId) {
            if (entityIdToPoolIndex.find(entityId) == entityIdToPoolIndex.end()) {
                throw std::out_of_range("EntityId not found in Pool");
            }

            return data[entityIdToPoolIndex[entityId]];
        }

        void Remove(int entityId) {
            if (entityIdToPoolIndex.find(entityId) == entityIdToPoolIndex.end()) {
                return;
            }

            int lastIndex = size - 1;
            int entityIdOfLast = poolIndexToEntityId[lastIndex];

            int toDeletePoolIndex = entityIdToPoolIndex[entityId];
            
            data[toDeletePoolIndex] = data[lastIndex];
            size --;
            entityIdToPoolIndex[entityIdOfLast] = toDeletePoolIndex;
            entityIdToPoolIndex.erase(entityId);
            poolIndexToEntityId[toDeletePoolIndex] = entityIdOfLast;
        }

        void RemoveEntityComponent(int entityId) {
            Remove(entityId);
        }
};

// the Entity Manager that coordinates everything
class Registry {
    private:
        int numEntities = 0;
        std::deque<int> freeEntityIds;

        std::vector<std::shared_ptr<IPool>> componentPools; 

        std::vector<Signature> entityComponentSignatures;

        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

        // entities that are waited to be added / deleted
        std::set<Entity> entitiesToAdd;
        std::set<Entity> entitiesToRemove;

        // 管理entity的tag和group
		std::unordered_map<std::string, Entity> entityPerTag;
        std::unordered_map<int, std::string> tagPerEntity;

		std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
        std::unordered_map<int, std::string> groupPerEntity;

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
        void RemoveEntity(Entity entity);
        
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

        void AddEntityToSystems(Entity entity);
        void RemoveEntityFromSystems(Entity entity);

        // Tag Management
        void AddTag(Entity entity, const std::string& tag);
        bool RemoveTag(Entity entity);
        bool HasTag(Entity entity, const std::string& tag) const;
        std::string GetTag(Entity entity) const;

        // Group Management
        void AddEntityToGroup(Entity entity, const std::string& group);
        bool RemoveEntityFromGroup(Entity entity);
        std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
        bool BelongToGroup(Entity entity, const std::string& group) const;
        std::string GetGroup(Entity entity) const;
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

    // if (entityId >= componentPool->GetSize()) {
    //     componentPool->Resize(numEntities);
    // }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityId, newComponent);

    if (entityComponentSignatures.size() <= entityId) {
        entityComponentSignatures.resize(numEntities);
    }
    entityComponentSignatures[entityId].set(componentId);

    // Logger::Log("Add Component " + std::string(typeid(TComponent).name()) + "id = " + std::to_string(componentId) + " to Entity id = " + std::to_string(entityId));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
    const auto entityId = entity.GetId();
    const auto componentId = Component<TComponent>::GetId();

    if (entityComponentSignatures[entityId].test(componentId)) {
        entityComponentSignatures[entityId].set(componentId, false);
    }

    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    componentPool->Remove(entityId);
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