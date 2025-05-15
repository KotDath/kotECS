#pragma once
#include <iterator>
#include <memory>
#include <span>
#include <vector>

#include "BaseComponentStorage.h"

class KotIterator {
 public:
  inline bool operator!=(const KotIterator &other) const {
    return _current != other._current;
  }

  using iterator_category = std::input_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = int;
  using pointer = int *;
  using reference = int &;

  // Constructor with all necessary parameters
  inline KotIterator(
      size_t current,
      const std::vector<std::shared_ptr<BaseComponentStorage>> &storages,
      const std::vector<std::shared_ptr<BaseComponentStorage>>
          &excludedStorages,
      std::span<const int> minStorageEntities, size_t minStorageIndex,
      bool isReversed = false)
      : _storages(storages),
        _excludedStorages(excludedStorages),
        _current(current),
        _minStorageEntities(minStorageEntities),
        _minStorageIndex(minStorageIndex),
        _currentEntity(-1) {
    for (const auto &entity : minStorageEntities) {
      _currentEntity = entity;
      if (HasAllComponents() && !HasExcludedComponents()) {
        break;
      }

      if (_current < minStorageEntities.size()) {
        ++_current;
      }
    }
  }

  inline KotIterator &operator++() {
    while (++_current < _minStorageEntities.size()) {
      _currentEntity = _minStorageEntities[_current];
      if (HasAllComponents() && !HasExcludedComponents()) {
        break;
      }
    }
    return *this;
  }

  inline int operator*() const { return _currentEntity; }

 private:
  const std::span<const int> _minStorageEntities;
  const std::vector<std::shared_ptr<BaseComponentStorage>> &_storages;
  const std::vector<std::shared_ptr<BaseComponentStorage>> &_excludedStorages;
  const size_t _minStorageIndex;
  int _currentEntity;
  size_t _current;

  // Helper method to check if the current entity exists in all component
  // storages
  inline bool HasAllComponents() const {
    for (size_t i = 0; i < _storages.size(); ++i) {
      if (i == _minStorageIndex)
        continue;
      if (!_storages[i]->Has(_currentEntity)) {
        return false;
      }
    }
    return true;
  }

  inline bool HasExcludedComponents() const {
    for (const auto &storage : _excludedStorages) {
      if (storage->Has(_currentEntity)) {
        return true;
      }
    }
    return false;
  }
};
