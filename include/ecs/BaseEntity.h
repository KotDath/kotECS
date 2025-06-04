#pragma once

#include <iostream>
#include <string>

struct BaseEntity {
  std::string name;

  friend std::ostream& operator<<(std::ostream& os, const BaseEntity& entity) {
    os << "BaseEntity(name=" << entity.name << ")";
    return os;
  }
};