#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

class EntityId {
 public:
  const int Id;

  inline EntityId(int id, int gen) : _gen(gen), Id(id) {
    _components.reserve(6);
  }

  inline void AddComponent(int component) { _components.push_back(component); }

  inline int RemoveComponent(int component) {
    auto it = std::find(_components.begin(), _components.end(), component);
    if (it != _components.end()) {
      _components.erase(it);
      return 1;
    }
    return 0;
  }

  inline bool IsRemoved() const { return _gen < 0; }

  inline const std::vector<int>& Components() const { return _components; }

  inline int Gen() const { return _gen; }

  inline void Remove() {
    _gen = -(_gen + 1);
    _components.clear();
  }

  inline void Recycle() { _gen *= -1; }

  inline bool Equals(const EntityId& other) const {
    return Id == other.Id && _gen == other._gen;
  }

  inline bool operator==(const EntityId& other) const { return Equals(other); }

  inline bool operator!=(const EntityId& other) const { return !Equals(other); }

  friend std::ostream& operator<<(std::ostream& os, const EntityId& eId);

 private:
  std::vector<int> _components;
  int _gen;
};

inline std::ostream& operator<<(std::ostream& os, const EntityId& eId) {
  os << eId.Id << "(" << eId.Gen() << ")";
  return os;
}
