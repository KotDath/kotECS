#include <ecs/BaseEntity.h>
#include <ecs/World.h>
#include <gtest/gtest.h>

TEST(BaseEntityTest, NewEntityHasBaseEntityComponent) {
  World world;

  // Create a new entity
  int entityId = world.CreateEntity();

  // Verify that the entity has a BaseEntity component
  auto& baseEntityStorage = world.GetStorage<BaseEntity>();
  ASSERT_TRUE(baseEntityStorage.Has(entityId));

  // Verify the BaseEntity component's properties
  auto& baseEntity = baseEntityStorage.Get(entityId);
  EXPECT_EQ(baseEntity.name, "Entity_" + std::to_string(entityId));
}

TEST(BaseEntityTest, MultipleEntitiesHaveUniqueBaseEntityComponents) {
  World world;

  // Create multiple entities
  int entity1 = world.CreateEntity();
  int entity2 = world.CreateEntity();
  int entity3 = world.CreateEntity();

  // Verify each entity has a unique BaseEntity component
  auto& baseEntityStorage = world.GetStorage<BaseEntity>();

  EXPECT_EQ(baseEntityStorage.Get(entity1).name,
            "Entity_" + std::to_string(entity1));
  EXPECT_EQ(baseEntityStorage.Get(entity2).name,
            "Entity_" + std::to_string(entity2));
  EXPECT_EQ(baseEntityStorage.Get(entity3).name,
            "Entity_" + std::to_string(entity3));
}

TEST(BaseEntityTest, RecycledEntityGetsNewBaseEntityComponent) {
  World world;

  // Create an entity
  int entityId = world.CreateEntity();
  auto& baseEntityStorage = world.GetStorage<BaseEntity>();

  // Store the original name
  std::string originalName = baseEntityStorage.Get(entityId).name;

  // Remove the entity
  world.RemoveEntity(entityId);

  // Create a new entity (should reuse the same ID)
  int newEntityId = world.CreateEntity();

  // Verify it's the same ID
  EXPECT_EQ(entityId, newEntityId);

  // Verify the BaseEntity component has been updated
  EXPECT_NE(baseEntityStorage.Get(newEntityId).name, originalName);
  EXPECT_EQ(baseEntityStorage.Get(newEntityId).name,
            "Entity_" + std::to_string(newEntityId));
}