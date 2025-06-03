#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

class EntityId {
 public:
  const int Id;

  inline EntityId(int id, int gen) : _gen(gen), Id(id) {
    _componentHashes.reserve(6);
  }

  inline void AddComponent(size_t componentHash) { 
    _componentHashes.push_back(componentHash); 
  }

  inline int RemoveComponent(size_t componentHash) {
    auto it = std::find(_componentHashes.begin(), _componentHashes.end(), componentHash);
    if (it != _componentHashes.end()) {
      _componentHashes.erase(it);
      return _componentHashes.size();
    }
    return _componentHashes.size();
  }

  inline bool IsRemoved() const { return _gen < 0; }

  inline const std::vector<size_t>& ComponentHashes() const { return _componentHashes; }

  inline int Gen() const { return _gen; }

  inline void Remove() {
    _gen = -(_gen + 1);
    _componentHashes.clear();
  }

  inline void Recycle() { _gen *= -1; }

  inline bool Equals(const EntityId& other) const {
    return Id == other.Id && _gen == other._gen;
  }

  inline bool operator==(const EntityId& other) const { return Equals(other); }

  inline bool operator!=(const EntityId& other) const { return !Equals(other); }

  friend std::ostream& operator<<(std::ostream& os, const EntityId& eId);

 private:
  std::vector<size_t> _componentHashes;
  int _gen;
};

inline std::ostream& operator<<(std::ostream& os, const EntityId& eId) {
  os << eId.Id << "(" << eId.Gen() << ")";
  return os;
}
