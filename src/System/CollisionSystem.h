#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../Toolbox/BVH.h"
#include "../Components/Collidable.h"
#include "../Components/Transform.h"
#include "../Components/AABB.h"
#include "System.h"
#include <vector>

class CollisionSystem : public System {

public:
  CollisionSystem();
  void Update();
  virtual void NotifyAdded(Entity e);
  virtual void NotifyErased(Entity e);
private:
  const float epsilon = 0.1;
  BVH m_bvh;
};
#endif
