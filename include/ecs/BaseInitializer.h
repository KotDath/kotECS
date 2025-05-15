#pragma once

#include "World.h"

class BaseInitializer {
 public:
  BaseInitializer(World& world) : world(world) {}
  const World& GetWorld() const { return world; }
  virtual ~BaseInitializer() = default;
  virtual void OnInit() = 0;

 protected:
  World& world;
};
