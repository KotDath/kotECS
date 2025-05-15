#pragma once

#include <memory>
#include <vector>

#include "BaseComponentStorage.h"
#include "Filter.h"
#include "World.h"

class FilterBuilder {
 public:
  FilterBuilder(World& world) : _world(world) { _componentStorages.reserve(4); }
  template <typename T>
  FilterBuilder& With() {
    auto storage = _world.GetRawStorage<T>();
    _componentStorages.push_back(storage);
    return *this;
  }
  Filter Build() const { return Filter(_componentStorages); }

 private:
  World& _world;
  std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;
};
