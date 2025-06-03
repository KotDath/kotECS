#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <span>
#include <type_traits>
#include <vector>

#include "BaseComponentStorage.h"

// Forward declaration
class World;

template <typename T>
class ComponentStorage : public BaseComponentStorage {
 public:
  using ComponentChangeCallback = std::function<void(int entityId, size_t typeHash, bool added)>;

  ComponentStorage(ComponentChangeCallback callback) : _onComponentChange(callback) { 
    Resize(64, 64); 
  }

  bool Has(const int entityIid) const override {
    return entityIid < _sparse.size() && _sparse[entityIid] != -1;
  }

  T& Get(const int entityIid) {
    if (entityIid >= _sparse.size() || _sparse[entityIid] == -1) {
      throw std::out_of_range("Entity ID is out of range");
    }
    return _data[_sparse[entityIid]];
  }

  void Add(const int entityIid, const T& value) {
    Resize((entityIid / 64 + 1) * 64,
           _data.size() == _count + 1 ? _data.size() + 64 : _data.size());
    _data[_count] = value;
    _dense[_count] = entityIid;
    _sparse[entityIid] = _count;
    _count++;

    if (_onComponentChange) {
      _onComponentChange(entityIid, typeid(T).hash_code(), true);
    }
  }

  void Remove(const int entityIid) override {
    int arrayIndex = _sparse[entityIid];
    if (arrayIndex == -1) {
      return;
    }
    int lastEntityIid = _dense[--_count];
    _data[arrayIndex] = _data[_count];
    _dense[arrayIndex] = lastEntityIid;
    _sparse[lastEntityIid] = arrayIndex;
    _sparse[entityIid] = -1;

    if (_onComponentChange) {
      _onComponentChange(entityIid, typeid(T).hash_code(), false);
    }
  }

  const std::span<const T> All() const {
    return std::span(_data.begin(), _count);
  }

  std::span<const int> Entities() const override {
    return std::span(_dense.begin(), _count);
  }

  int Count() const override { return _count; }

  template <typename U>
  friend std::ostream& operator<<(std::ostream& os,
                                  const ComponentStorage<U>& storage);

 private:
  std::vector<T> _data;
  std::vector<int> _sparse;
  std::vector<int> _dense;
  ComponentChangeCallback _onComponentChange;

  int _count = 0;

  void Resize(const int sparseSize, const int dataSize) {
    int oldSparseSize = _sparse.size();
    if (oldSparseSize < sparseSize) {
      _sparse.resize(sparseSize);
      std::fill_n(_sparse.data() + oldSparseSize, sparseSize - oldSparseSize,
                  -1);
    }
    int oldDataSize = _dense.size();
    if (oldDataSize < dataSize) {
      _dense.resize(dataSize);
      _data.resize(dataSize);
      std::fill_n(_dense.data() + oldDataSize, dataSize - oldDataSize, -1);
    }
  }
};

// Implementation of stream output operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const ComponentStorage<T>& storage) {
  os << "ComponentStorage<" << typeid(T).name() << "> [";
  for (int i = 0; i < storage._count; ++i) {
    if (i > 0)
      os << ", ";
    os << "Entity " << storage._dense[i] << ": " << storage._data[i];
  }
  os << "]";
  return os;
}
