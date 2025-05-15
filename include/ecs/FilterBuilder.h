#pragma once

#include <memory>
#include <vector>

#include "BaseComponentStorage.h"
#include "Filter.h"
#include "World.h"

class FilterBuilder {
 public:
  FilterBuilder(World& world) : _world(world) {
    _componentStorages.reserve(4);
    _componentStorages.push_back(_world.GetRawStorage<BaseEntity>());
    _excludedStorages.reserve(4);
  }

  template <typename T>
  FilterBuilder& With() {
    auto storage = _world.GetRawStorage<T>();
    _componentStorages.push_back(storage);
    return *this;
  }

  template <typename T>
  FilterBuilder& Without() {
    auto storage = _world.GetRawStorage<T>();
    _excludedStorages.push_back(storage);
    return *this;
  }

  Filter Build() const { return Filter(_componentStorages, _excludedStorages); }

 private:
  World& _world;
  std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;
  std::vector<std::shared_ptr<BaseComponentStorage>> _excludedStorages;
};
