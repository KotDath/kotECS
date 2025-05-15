#pragma once

#include <iostream>
#include <string>

struct BaseEntity {
  std::string name;
  bool isActive = true;

  friend std::ostream& operator<<(std::ostream& os, const BaseEntity& entity) {
    os << "BaseEntity(name=" << entity.name << ", isActive=" << entity.isActive
       << ")";
    return os;
  }
};