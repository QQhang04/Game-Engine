#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
    protected:
        static int nextId;
};

template<typename TComponent>
class Component : public IComponent {
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
        bool operator==(const Entity& other) const { return id == other.id;}
        bool operator!=(const Entity& other) const { return id != other.id;}
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

        std::vector<IPool*> componentPools; 

        std::vector<Signature> entityComponentSignatures;

        std::unordered_map<std::type_index, System*> systems;

        // entities that are waited to be added / deleted
        std::set<Entity> entitiesToAdd;
        std::set<Entity> entitiesToRemove;

    public:
        Registry() = default;

        void Update();

        // TODO:
        // create a new entity
        Entity CreateEntity();

        // only the entity can link to this system
        void AddEntityToSystem(Entity entity);

        // kill entity

        // add a component to entity: AddComponent<T>();
        
        // remove a component from entity
        // does a entity has a component

        // add system
        // remove system
        // has system
        // get system
};

template<typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}



#endif