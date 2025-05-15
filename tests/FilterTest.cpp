#include <gtest/gtest.h>

#include "ecs/Filter.h"
#include "ecs/FilterBuilder.h"
#include "ecs/World.h"

// Test components
struct Position {
  float x, y;
};

struct Velocity {
  float dx, dy;
};

struct Health {
  int current;
  int max;
};

class FilterTest : public ::testing::Test {
 protected:
  World world;

  void SetUp() override {
    // Create some test entities with different component combinations
    int e1 = world.CreateEntity();
    int e2 = world.CreateEntity();
    int e3 = world.CreateEntity();
    int e4 = world.CreateEntity();

    // e1: Position + Velocity
    world.GetStorage<Position>().Add(e1, Position{10.0f, 20.0f});
    world.GetStorage<Velocity>().Add(e1, Velocity{1.0f, 2.0f});

    // e2: Position + Health
    world.GetStorage<Position>().Add(e2, Position{30.0f, 40.0f});
    world.GetStorage<Health>().Add(e2, Health{100, 100});

    // e3: Position + Velocity + Health
    world.GetStorage<Position>().Add(e3, Position{50.0f, 60.0f});
    world.GetStorage<Velocity>().Add(e3, Velocity{3.0f, 4.0f});
    world.GetStorage<Health>().Add(e3, Health{75, 100});

    // e4: Position only
    world.GetStorage<Position>().Add(e4, Position{70.0f, 80.0f});
  }
};

TEST_F(FilterTest, SingleComponentFilter) {
  auto filter = FilterBuilder(world).With<Position>().Build();

  std::vector<int> entities;
  for (int entity : filter) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), 4);
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 0) != entities.end());
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 1) != entities.end());
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 2) != entities.end());
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 3) != entities.end());
}

TEST_F(FilterTest, TwoComponentFilter) {
  auto filter = FilterBuilder(world).With<Position>().With<Velocity>().Build();

  std::vector<int> entities;
  for (int entity : filter) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), 2);
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 0) != entities.end());
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 2) != entities.end());
}

TEST_F(FilterTest, ThreeComponentFilter) {
  auto filter = FilterBuilder(world)
                    .With<Position>()
                    .With<Velocity>()
                    .With<Health>()
                    .Build();

  std::vector<int> entities;
  for (int entity : filter) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), 1);
  EXPECT_EQ(entities[0], 2);
}

TEST_F(FilterTest, EmptyFilter) {
  auto filter = FilterBuilder(world).Build();

  std::vector<int> entities;
  for (int entity : filter) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), 4);
}

TEST_F(FilterTest, ComponentRemoval) {
  // First verify entity 0 has Position and Velocity
  auto initialFilter =
      FilterBuilder(world).With<Position>().With<Velocity>().Build();

  std::vector<int> initialEntities;
  for (int entity : initialFilter) {
    initialEntities.push_back(entity);
  }
  EXPECT_EQ(initialEntities.size(), 2);
  EXPECT_TRUE(std::find(initialEntities.begin(), initialEntities.end(), 0) !=
              initialEntities.end());

  // Remove Velocity from entity 0
  world.GetStorage<Velocity>().Remove(0);

  // Check that entity 0 is no longer in the filter
  std::vector<int> finalEntities;
  for (int entity : initialFilter) {
    finalEntities.push_back(entity);
  }
  EXPECT_EQ(finalEntities.size(), 1);
  EXPECT_TRUE(std::find(finalEntities.begin(), finalEntities.end(), 0) ==
              finalEntities.end());
}

TEST_F(FilterTest, WithoutComponent) {
  // Get all entities that have Position but don't have Health
  auto filter = FilterBuilder(world).With<Position>().Without<Health>().Build();

  std::vector<int> entities;
  for (int entity : filter) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), 2);
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 0) !=
              entities.end());  // e1
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 3) !=
              entities.end());  // e4
}

TEST_F(FilterTest, WithoutMultipleComponents) {
  // Get all entities that have Position but don't have Velocity or Health
  auto filter = FilterBuilder(world)
                    .With<Position>()
                    .Without<Velocity>()
                    .Without<Health>()
                    .Build();

  std::vector<int> entities;
  for (int entity : filter) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), 1);
  EXPECT_EQ(entities[0], 3);  // e4
}

TEST_F(FilterTest, WithoutOnly) {
  // Get all entities that don't have Health
  auto filter = FilterBuilder(world).Without<Health>().Build();

  std::vector<int> entities;
  for (int entity : filter) {
    entities.push_back(entity);
  }

  EXPECT_EQ(entities.size(), 2);
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 0) !=
              entities.end());  // e1
  EXPECT_TRUE(std::find(entities.begin(), entities.end(), 3) !=
              entities.end());  // e4
}