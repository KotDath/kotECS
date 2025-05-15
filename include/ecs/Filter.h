#pragma once

#include <cstdio>
#include <memory>
#include <vector>

#include "BaseComponentStorage.h"
#include "KotIterator.h"

class Filter {
 public:
  Filter(const std::vector<std::shared_ptr<BaseComponentStorage>>&
             componentStorages,
         const std::vector<std::shared_ptr<BaseComponentStorage>>&
             excludedStorages)
      : _componentStorages(componentStorages),
        _excludedStorages(excludedStorages) {}

  inline KotIterator begin() {
    if (_componentStorages.empty() && _excludedStorages.empty()) {
      return KotIterator(0, _componentStorages, _excludedStorages, {}, -1);
    }
    _minStorageIndex = FindMinComponentStorage();
    const auto& minStorageEntities =
        _componentStorages[_minStorageIndex]->Entities();
    const int current = 0;
    return KotIterator(current, _componentStorages, _excludedStorages,
                       minStorageEntities, _minStorageIndex);
  }

  inline KotIterator end() {
    if (_componentStorages.empty() && _excludedStorages.empty()) {
      return KotIterator(0, _componentStorages, _excludedStorages, {}, -1);
    }
    _minStorageIndex = FindMinComponentStorage();
    const auto& minStorageEntities =
        _componentStorages[_minStorageIndex]->Entities();
    const int current = minStorageEntities.size();
    return KotIterator(current, _componentStorages, _excludedStorages,
                       minStorageEntities, _minStorageIndex);
  }

  friend class KotIterator;

 private:
  std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;
  std::vector<std::shared_ptr<BaseComponentStorage>> _excludedStorages;
  size_t _minStorageIndex = -1;

  inline size_t FindMinComponentStorage() const {
    if (_componentStorages.empty()) {
      return -1;
    }
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
