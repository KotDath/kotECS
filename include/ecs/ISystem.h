#pragma once

#include "IInitializer.h"

class ISystem : public IInitializer {
 public:
  ISystem(World& world) : IInitializer(world) {}
  ~ISystem() override = default;
  virtual void OnUpdate() = 0;
};