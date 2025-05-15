#pragma once

#include "World.h"

class IInitializer {
 protected:
  World& world;

 public:
  IInitializer(World& world) : world(world) {}
  World& GetWorld() { return world; }
  virtual ~IInitializer() = default;
  virtual void OnInit() = 0;
};