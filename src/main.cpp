#include <iostream>

#include "kotECS.h"

// Test components
struct Position {
  float x, y;
};

std::ostream& operator<<(std::ostream& os, const Position& position) {
  os << "Position(" << position.x << ", " << position.y << ")";
  return os;
}

struct Velocity {
  float dx, dy;
};

std::ostream& operator<<(std::ostream& os, const Velocity& velocity) {
  os << "Velocity(" << velocity.dx << ", " << velocity.dy << ")";
  return os;
}

struct Health {
  int current;
  int max;
};

std::ostream& operator<<(std::ostream& os, const Health& health) {
  os << "Health(" << health.current << ", " << health.max << ")";
  return os;
}

int main() {
  World world;
  int e1 = world.CreateEntity();
  int e2 = world.CreateEntity();
  int e3 = world.CreateEntity();
  int e4 = world.CreateEntity();

  // e1: Position + Velocity
  world.GetStorage<Position>().Add(e1, Position{10.0f, 20.0f});
  world.GetStorage<Velocity>().Add(e1, Velocity{1.0f, 2.0f});

  world.GetStorage<Position>().Add(e4, Position{50.0f, 60.0f});
  world.GetStorage<Velocity>().Add(e4, Velocity{3.0f, 4.0f});
  // e2: Position + Health
  world.GetStorage<Position>().Add(e2, Position{30.0f, 40.0f});
  world.GetStorage<Health>().Add(e2, Health{100, 100});

  // e3: Position + Velocity + Health
  world.GetStorage<Position>().Add(e3, Position{50.0f, 60.0f});
  world.GetStorage<Velocity>().Add(e3, Velocity{3.0f, 4.0f});
  world.GetStorage<Health>().Add(e3, Health{75, 100});

  std::cout << world.GetStorage<Position>() << std::endl;
  std::cout << world.GetStorage<Velocity>() << std::endl;
  std::cout << world.GetStorage<Health>() << std::endl;

  auto initialFilter = FilterBuilder(world)
                           .With<Position>()
                           .With<Velocity>()
                           .With<Health>()
                           .Build();

  std::vector<int> initialEntities;
  for (int entity : initialFilter) {
    initialEntities.push_back(entity);
  }
  std::cout << "Initial entities: ";
  for (int entity : initialEntities) {
    std::cout << entity << " ";
  }
  std::cout << std::endl;

  // Remove the entity
  world.RemoveEntity(2);

  // Check that the entity is no longer in the filter
  std::vector<int> finalEntities;
  for (int entity : initialFilter) {
    finalEntities.push_back(entity);
  }
  std::cout << "Final entities: ";
  for (int entity : finalEntities) {
    std::cout << entity << " ";
  }
  std::cout << std::endl;
  return 0;
}