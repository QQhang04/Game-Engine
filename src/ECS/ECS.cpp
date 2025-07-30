#include "ECS.h"

int IComponent::nextId = 0;

// Entity impl
int Entity::GetId() const {
    return id;
}

void Entity::Destroy() const {
    Logger::Log("Entity::Destroy " + std::to_string(id));
    registry->RemoveEntity(*this);
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

// TODO 目前存在已经建立的entity被删掉、移除component, 或者增加component时对应system的更新不到位的情况
std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

// Registry impl
// TODO 1使用别的数据结构来优化算法
// TODO 2目前存在已经建立的entity被删掉、移除component, 或者增加component时对应system的更新不到位的情况
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

void Registry::RemoveEntityFromSystems(Entity entity) {
    for (auto& system : systems) {
        system.second->RemoveEntityFromSystem(entity);
    }
}

Entity Registry::CreateEntity() {
    int entityId;
    if (freeEntityIds.empty()) {
        entityId = numEntities++;
    } else {
        entityId = freeEntityIds.front();
        freeEntityIds.pop_front();
    }
    Entity entity(entityId);
    entity.registry = this;
    entitiesToAdd.insert(entity);

    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
    }

    // Logger::Log("Entity created with id: " + std::to_string(entityId));
    return entity;
}

void Registry::RemoveEntity(Entity entity) {
    entitiesToRemove.insert(entity);
}

void Registry::Update() {
    // add the entities in the waiting list
    for (auto entity : entitiesToAdd) {
        AddEntityToSystems(entity);
    }
    entitiesToAdd.clear();

    for (auto entity : entitiesToRemove) {
        RemoveEntityFromSystems(entity);
        
        // 清理tag和group信息，防止ID重用时造成混乱
        RemoveTag(entity);
        RemoveEntityFromGroup(entity);
        
        // 清理entity的component信息
        entityComponentSignatures[entity.GetId()].reset();
        for (auto pool : componentPools) {
            if (!pool){
                return;
            }
            pool -> RemoveEntityComponent(entity.GetId());
        }

        freeEntityIds.push_back(entity.GetId());
    }
    entitiesToRemove.clear();
}

// Tag Management
void Registry::AddTag(Entity entity, const std::string& tag) {
    // 如果entity已经有tag，先移除旧tag
    RemoveTag(entity);
    
    // 添加新tag
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
}

// TODO 判断时使用位运算优化性能
bool Registry::RemoveTag(Entity entity) {
    auto it = tagPerEntity.find(entity.GetId());
    if (it == tagPerEntity.end()) {
        return false;
    }
    std::string tag = it->second;
    entityPerTag.erase(tag);
    tagPerEntity.erase(entity.GetId());
    return true;
}

bool Registry::HasTag(Entity entity, const std::string& tag) const {
    auto it = entityPerTag.find(tag);
    return it != entityPerTag.end() && it->second == entity;
}

std::string Registry::GetTag(Entity entity) const {
    auto it = tagPerEntity.find(entity.GetId());
    if (it != tagPerEntity.end()) {
        return it->second;
    }
    return "";
}

// Group Management
void Registry::AddEntityToGroup(Entity entity, const std::string& group) {
    // 如果entity已经属于某个group，先从旧group中移除
    RemoveEntityFromGroup(entity);
    
    // 添加到新group
    entitiesPerGroup[group].insert(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::RemoveEntityFromGroup(Entity entity) {
    auto it = groupPerEntity.find(entity.GetId());
    if (it == groupPerEntity.end()) {
        return false;
    }
    std::string group = it->second;
    entitiesPerGroup[group].erase(entity);
    groupPerEntity.erase(entity.GetId());
    return true;
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const {
    auto it = entitiesPerGroup.find(group);
    if (it == entitiesPerGroup.end()) {
        return {};
    }
    return std::vector<Entity>(it->second.begin(), it->second.end());
}

bool Registry::BelongToGroup(Entity entity, const std::string& group) const {
    auto it = groupPerEntity.find(entity.GetId());
    return it != groupPerEntity.end() && it->second == group;
}

std::string Registry::GetGroup(Entity entity) const {
    auto it = groupPerEntity.find(entity.GetId());
    if (it != groupPerEntity.end()) {
        return it->second;
    }
    return "";
}

// Entity Tag/Group 相关成员函数实现
void Entity::AddTag(const std::string& tag) {
    if (registry) {
        registry->AddTag(*this, tag);
    }
}

bool Entity::RemoveTag() {
    if (registry) {
        return registry->RemoveTag(*this);
    }
    return false;
}

bool Entity::HasTag(const std::string& tag) const {
    if (registry) {
        return registry->HasTag(*this, tag);
    }
    return false;
}

std::string Entity::GetTag() const {
    if (registry) {
        return registry->GetTag(*this);
    }
    return "";
}

void Entity::AddToGroup(const std::string& group) {
    if (registry) {
        registry->AddEntityToGroup(*this, group);
    }
}

bool Entity::RemoveFromGroup() {
    if (registry) {
        return registry->RemoveEntityFromGroup(*this);
    }
    return false;
}

bool Entity::BelongToGroup(const std::string& group) const {
    if (registry) {
        return registry->BelongToGroup(*this, group);
    }
    return false;
}

std::string Entity::GetGroup() const {
    if (registry) {
        return registry->GetGroup(*this);
    }
    return "";
}