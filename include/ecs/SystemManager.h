#pragma once

#include <memory>
#include <vector>

#include "IInitializer.h"
#include "ISystem.h"
#include "World.h"

class SystemManager {
 public:
  explicit SystemManager(World& world) : _world(world) {}

  SystemManager& AddInitializer(std::shared_ptr<IInitializer> initializer) {
    _newInitializers.push_back(initializer);
    return *this;
  }

  SystemManager& AddSystem(std::shared_ptr<ISystem> system) {
    _systems.push_back(system);
    _newInitializers.push_back(system);
    return *this;
  }

  void Initialize() {
    // Process new initializers
    for (const auto& initializer : _newInitializers) {
      initializer->OnInit();
      _initializers.push_back(initializer);
    }
    _newInitializers.clear();
  }

  void Update() {
    Initialize();
    for (int i = 0; i < _systems.size(); i++) {
      _systems[i]->OnUpdate();
    }

    auto movementStorage = _world.GetStorage<C_Asteroid>();
    std::cout << "Movement storage: " << movementStorage << std::endl;
  }

 private:
  World& _world;
  std::vector<std::shared_ptr<IInitializer>> _newInitializers;
  std::vector<std::shared_ptr<IInitializer>> _initializers;
  std::vector<std::shared_ptr<ISystem>> _systems;
};
