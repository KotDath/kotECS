#pragma once

#include <cstdio>
#include <memory>
#include <vector>

#include "BaseComponentStorage.h"
#include "KotIterator.h"

class Filter {
 public:
  Filter(const std::vector<std::shared_ptr<BaseComponentStorage>>&
             componentStorages)
      : _componentStorages(componentStorages) {}

  inline KotIterator begin() {
    _minStorageIndex = FindMinComponentStorage();
    const auto& minStorageEntities =
        _componentStorages[_minStorageIndex]->Entities();
    const int current = 0;
    return KotIterator(current, _componentStorages, minStorageEntities,
                       _minStorageIndex);
  }

  inline KotIterator end() {
    _minStorageIndex = FindMinComponentStorage();
    const auto& minStorageEntities =
        _componentStorages[_minStorageIndex]->Entities();
    const int current = minStorageEntities.size();
    return KotIterator(current, _componentStorages, minStorageEntities,
                       _minStorageIndex);
  }

  friend class KotIterator;

 private:
  std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;
  size_t _minStorageIndex = -1;

  inline size_t FindMinComponentStorage() const {
    size_t minIndex = 0;
    size_t minCount = _componentStorages[0]->Count();
    for (size_t i = 1; i < _componentStorages.size(); ++i) {
      if (_componentStorages[i]->Count() < minCount) {
        minCount = _componentStorages[i]->Count();
        minIndex = i;
      }
    }
    return minIndex;
  }
};
