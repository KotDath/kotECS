#pragma once

#include "BaseInitializer.h"

class BaseSystem : public BaseInitializer {
 public:
  BaseSystem(World &world) : BaseInitializer(world) {}
  ~BaseSystem() override = default;
  virtual void OnUpdate() = 0;
};
