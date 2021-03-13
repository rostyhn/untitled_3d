#ifndef SYSTEM_H
#define SYSTEM_H

#include "../Toolbox/TypeDefinitions.h"
#include <set>

class System {
 public:
  std::set<Entity> m_Entities;
  virtual void NotifyAdded(const Entity e) {}
  virtual void NotifyErased(const Entity e) {}
};
#endif
