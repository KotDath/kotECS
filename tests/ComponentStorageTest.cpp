#include <gtest/gtest.h>

#include "ecs/ComponentStorage.h"

// Test fixture for ComponentStorage
template <typename T>
class ComponentStorageTest : public ::testing::Test {
 protected:
  ComponentStorage<T> storage;
};

// Test with different types
using TestTypes = ::testing::Types<int, float, std::string>;
TYPED_TEST_SUITE(ComponentStorageTest, TestTypes);

// Basic functionality tests
TYPED_TEST(ComponentStorageTest, InitialState) {
  EXPECT_EQ(this->storage.Count(), 0);
  EXPECT_FALSE(this->storage.Has(0));
}

TYPED_TEST(ComponentStorageTest, AddAndHas) {
  TypeParam value{};
  this->storage.Add(1, value);
  EXPECT_TRUE(this->storage.Has(1));
  EXPECT_FALSE(this->storage.Has(2));
  EXPECT_EQ(this->storage.Count(), 1);
}

TYPED_TEST(ComponentStorageTest, AddAndGet) {
  TypeParam value{};
  this->storage.Add(1, value);
  EXPECT_EQ(this->storage.Get(1), value);
}

TYPED_TEST(ComponentStorageTest, Remove) {
  TypeParam value{};
  this->storage.Add(1, value);
  this->storage.Remove(1);
  EXPECT_FALSE(this->storage.Has(1));
  EXPECT_EQ(this->storage.Count(), 0);
}

TYPED_TEST(ComponentStorageTest, MultipleAddAndRemove) {
  TypeParam value1{};
  TypeParam value2{};

  this->storage.Add(1, value1);
  this->storage.Add(2, value2);
  EXPECT_EQ(this->storage.Count(), 2);

  this->storage.Remove(1);
  EXPECT_FALSE(this->storage.Has(1));
  EXPECT_TRUE(this->storage.Has(2));
  EXPECT_EQ(this->storage.Count(), 1);
}

TYPED_TEST(ComponentStorageTest, All) {
  TypeParam value1{};
  TypeParam value2{};

  this->storage.Add(1, value1);
  this->storage.Add(2, value2);

  auto all = this->storage.All();
  EXPECT_EQ(all.size(), 2);
}

TYPED_TEST(ComponentStorageTest, Entities) {
  TypeParam value{};

  this->storage.Add(1, value);
  this->storage.Add(2, value);

  auto entities = this->storage.Entities();
  EXPECT_EQ(entities.size(), 2);
  EXPECT_EQ(entities[0], 1);
  EXPECT_EQ(entities[1], 2);
}

// Specific type tests
TEST(ComponentStorageIntTest, ValueModification) {
  ComponentStorage<int> storage;
  storage.Add(1, 42);
  storage.Get(1) = 100;
  EXPECT_EQ(storage.Get(1), 100);
}

TEST(ComponentStorageStringTest, StringOperations) {
  ComponentStorage<std::string> storage;
  storage.Add(1, "hello");
  storage.Add(2, "world");

  EXPECT_EQ(storage.Get(1), "hello");
  EXPECT_EQ(storage.Get(2), "world");

  storage.Get(1) += " there";
  EXPECT_EQ(storage.Get(1), "hello there");
}

// Edge cases
TEST(ComponentStorageEdgeCases, LargeEntityIds) {
  ComponentStorage<int> storage;
  storage.Add(1000, 42);
  EXPECT_TRUE(storage.Has(1000));
  EXPECT_EQ(storage.Get(1000), 42);
}

TEST(ComponentStorageEdgeCases, RemoveNonExistent) {
  ComponentStorage<int> storage;
  storage.Add(1, 42);
  storage.Remove(2);  // Should not crash
  EXPECT_TRUE(storage.Has(1));
  EXPECT_EQ(storage.Count(), 1);
}

TEST(ComponentStorageEdgeCases, RefNonExistent) {
  ComponentStorage<int> storage;
  EXPECT_THROW(storage.Get(1), std::out_of_range);
}

// Stream output tests
TEST(ComponentStorageStreamTest, IntOutput) {
  ComponentStorage<int> storage;
  storage.Add(1, 42);
  storage.Add(2, 100);

  std::stringstream ss;
  ss << storage;
  std::string output = ss.str();

  EXPECT_TRUE(output.find("ComponentStorage") != std::string::npos);
  EXPECT_TRUE(output.find("42") != std::string::npos);
  EXPECT_TRUE(output.find("100") != std::string::npos);
}

// Custom type test
struct TestComponent {
  int x;
  float y;

  bool operator==(const TestComponent& other) const {
    return x == other.x && y == other.y;
  }

  friend std::ostream& operator<<(std::ostream& os, const TestComponent& tc) {
    return os << "(" << tc.x << "," << tc.y << ")";
  }
};

TEST(ComponentStorageCustomTypeTest, CustomTypeOperations) {
  ComponentStorage<TestComponent> storage;
  TestComponent tc{42, 3.14f};

  storage.Add(1, tc);
  EXPECT_TRUE(storage.Has(1));
  EXPECT_EQ(storage.Get(1), tc);

  storage.Get(1).x = 100;
  EXPECT_EQ(storage.Get(1).x, 100);
  EXPECT_EQ(storage.Get(1).y, 3.14f);
}