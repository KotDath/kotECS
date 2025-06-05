#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "BaseEntity.h"
#include "ComponentStorage.h"
#include "EntityId.h"

class World {
 public:
  inline World() {
    _entities.reserve(DefaultEntitiesCapacity);
    _freeEntities.reserve(DefaultEntitiesCapacity);
    // Initialize BaseEntity storage
    GetRawStorage<BaseEntity>();
  }

  inline int CreateEntity() {
    int entityId = 0;
    if (!_freeEntities.empty()) {
      entityId = _freeEntities.back();
      _freeEntities.pop_back();
      _entities[entityId].Recycle();
    } else {
      entityId = _entities.size();
      _entities.emplace_back(entityId, 1);
    }

    // Add BaseEntity component to the new entity
    GetStorage<BaseEntity>().Add(
        entityId, BaseEntity{"Entity_" + std::to_string(entityId)});

    return entityId;
  }

  inline void RemoveEntity(int ent) {
    auto& entity = _entities[ent];
    if (entity.IsRemoved())
      return;
    const auto componentHashes = entity.ComponentHashes();
    for (const auto& hash : componentHashes) {
        _componentStoragesHash[hash]->Remove(ent);
      }
    entity.Remove();
    _freeEntities.push_back(entity.Id);
    
  }

  inline EntityId GetPackedEntity(int e) const { return _entities[e]; }

  inline bool UnpackEntity(const EntityId& eId, int& e) const {
    e = eId.Id;
    return IsEntityAlive(e) && eId.Gen() == _entities[e].Gen();
  }

  inline bool IsEntityAlive(int e) const {
    auto& entity = _entities[e];
    return !entity.IsRemoved() && entity.Id > 0 &&
           entity.Id < _entities.size() && e == entity.Id;
  }

  inline void EntityComponentsChanged(const int ent, const size_t typeHash,
                                      const bool added) {
    auto& entity = _entities[ent];
    
    if (added) {
      entity.AddComponent(typeHash);
    } else {
      const int newComponentsCount = entity.RemoveComponent(typeHash);
      if (newComponentsCount == 0) {
        RemoveEntity(ent);
      }
    }
  }

  template <typename T>
  std::shared_ptr<ComponentStorage<T>> GetRawStorage() {
    const auto typeHash = typeid(T).hash_code();
    const auto foundStorageIterator = _componentStoragesHash.find(typeHash);
    if (foundStorageIterator != _componentStoragesHash.end())
      return std::static_pointer_cast<ComponentStorage<T>>(
          foundStorageIterator->second);
    int storagesCount = _componentStorages.size();
        
    // Create callback for component changes
    auto callback = [this](int entityId, size_t typeHash, bool added) {
      this->EntityComponentsChanged(entityId, typeHash, added);
    };
    
    auto storage = std::make_shared<ComponentStorage<T>>(callback);
    storagesCount++;
    _componentStoragesHash.insert({typeHash, storage});
    if (storagesCount == _componentStorages.capacity()) {
      const int newSize = _storagesCount << 1;
      _componentStorages.reserve(newSize);
    }
    _componentStorages.push_back(storage);
    return storage;
  }

  template <typename T>
  ComponentStorage<T>& GetStorage() {
    return *GetRawStorage<T>();
  }

 private:
  const int DefaultEntitiesCapacity = 64;
  std::vector<EntityId> _entities;
  std::vector<int> _freeEntities;
  std::unordered_map<size_t, std::shared_ptr<BaseComponentStorage>>
      _componentStoragesHash;
  std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;
  int _storagesCount = 0;
};
