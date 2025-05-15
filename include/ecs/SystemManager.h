#pragma once

#include <memory>
#include <vector>

#include "BaseInitializer.h"
#include "BaseSystem.h"
#include "World.h"

class SystemManager {
 public:
  SystemManager(World& world) : _world(world) {}
  SystemManager& AddInitializer(std::shared_ptr<BaseInitializer> initializer);
  SystemManager& AddSystem(std::shared_ptr<BaseSystem> system);
  void Initialize();
  void Update();

 private:
  std::vector<std::shared_ptr<BaseInitializer>> _newInitializers;
  std::vector<std::shared_ptr<BaseInitializer>> _initializers;
  std::vector<std::shared_ptr<BaseSystem>> _systems;
  World& _world;
};
