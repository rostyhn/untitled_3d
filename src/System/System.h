#ifndef SYSTEM_H
#define SYSTEM_H

#include "../Toolbox/TypeDefinitions.h"
#include <set>

class System {
 public:
  std::set<Entity> m_Entities;
};
#endif
