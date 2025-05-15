#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "ecs/ComponentStorage.h"
#include "ecs/FilterBuilder.h"
#include "ecs/ISystem.h"
#include "ecs/World.h"

struct PositionComponent {
  float X;
  float Y;

  PositionComponent() : X(0.0f), Y(0.0f) {}
  PositionComponent(float x, float y) : X(x), Y(y) {}
};

struct VelocityComponent {
  float X;
  float Y;

  VelocityComponent() : X(0.0f), Y(0.0f) {}
  VelocityComponent(float x, float y) : X(x), Y(y) {}
};

class PositionInitializer : public IInitializer {
 public:
  PositionInitializer(World& world) : IInitializer(world) {}

  void OnInit() override {
    const int player = world.CreateEntity();
    auto& storage = GetWorld().GetStorage<PositionComponent>();
    storage.Add(player, PositionComponent(10.0f, 20.0f));
  }
};

// Custom system that updates positions based on velocity
class MovementSystem : public ISystem {
 public:
  MovementSystem(World& world) : ISystem(world) {}

  void OnUpdate() override {
    auto filter = FilterBuilder(GetWorld())
                      .With<PositionComponent>()
                      .With<VelocityComponent>()
                      .Build();

    for (int entity : filter) {
      auto& pos = GetWorld().GetStorage<PositionComponent>().Get(entity);
      const auto& vel = GetWorld().GetStorage<VelocityComponent>().Get(entity);
      pos.X += vel.X;
      pos.Y += vel.Y;
    }
  }

  void OnInit() override {}
};

class SystemTest : public ::testing::Test {
 protected:
  World world;
  std::shared_ptr<MovementSystem> movementSystem;
  std::shared_ptr<PositionInitializer> positionInitializer;

  void SetUp() override {
    movementSystem = std::make_shared<MovementSystem>(world);
    positionInitializer = std::make_shared<PositionInitializer>(world);
  }
};

TEST_F(SystemTest, InitializerSetsCorrectPosition) {
  positionInitializer->OnInit();
  auto filter = FilterBuilder(world).With<PositionComponent>().Build();

  ASSERT_TRUE(filter.begin() != filter.end());

  const auto& pos =
      world.GetStorage<PositionComponent>().Get(0);  // First entity
  EXPECT_FLOAT_EQ(pos.X, 10.0f);
  EXPECT_FLOAT_EQ(pos.Y, 20.0f);
}

TEST_F(SystemTest, SystemUpdatesPositionBasedOnVelocity) {
  // Initialize position
  positionInitializer->OnInit();
  int entity = 0;  // First entity

  // Add velocity component
  auto& velStorage = world.GetStorage<VelocityComponent>();
  velStorage.Add(entity, VelocityComponent(1.0f, 2.0f));

  // Run system update
  movementSystem->OnUpdate();

  // Check if position was updated correctly
  auto& posStorage = world.GetStorage<PositionComponent>();
  const auto& pos = posStorage.Get(entity);
  EXPECT_FLOAT_EQ(pos.X, 11.0f);  // 10.0 + 1.0
  EXPECT_FLOAT_EQ(pos.Y, 22.0f);  // 20.0 + 2.0
}

TEST_F(SystemTest, SystemOnlyUpdatesEntitiesWithBothComponents) {
  // Initialize first entity
  positionInitializer->OnInit();
  int entity1 = 0;  // First entity

  // Create and initialize second entity
  int entity2 = world.CreateEntity();
  auto& posStorage = world.GetStorage<PositionComponent>();
  posStorage.Add(entity2, PositionComponent(10.0f, 20.0f));

  // Add velocity only to entity1
  auto& velStorage = world.GetStorage<VelocityComponent>();
  velStorage.Add(entity1, VelocityComponent(1.0f, 2.0f));

  // Run system update
  movementSystem->OnUpdate();

  // Check positions
  // Entity1 should be updated
  const auto& pos1 = posStorage.Get(entity1);
  EXPECT_FLOAT_EQ(pos1.X, 11.0f);
  EXPECT_FLOAT_EQ(pos1.Y, 22.0f);

  // Entity2 should remain unchanged
  const auto& pos2 = posStorage.Get(entity2);
  EXPECT_FLOAT_EQ(pos2.X, 10.0f);
  EXPECT_FLOAT_EQ(pos2.Y, 20.0f);
}

TEST_F(SystemTest, MultipleSystemUpdatesAccumulateChanges) {
  // Initialize position
  positionInitializer->OnInit();
  int entity = 0;  // First entity

  // Add velocity component
  auto& velStorage = world.GetStorage<VelocityComponent>();
  velStorage.Add(entity, VelocityComponent(1.0f, 2.0f));

  // Run system update multiple times
  movementSystem->OnUpdate();
  movementSystem->OnUpdate();
  movementSystem->OnUpdate();

  // Check if position was updated correctly
  auto& posStorage = world.GetStorage<PositionComponent>();
  const auto& pos = posStorage.Get(entity);
  EXPECT_FLOAT_EQ(pos.X, 13.0f);  // 10.0 + (1.0 * 3)
  EXPECT_FLOAT_EQ(pos.Y, 26.0f);  // 20.0 + (2.0 * 3)
}