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
private:
  std::vector<float> ReturnExtents(Entity e, Transform transform);
  BVH m_bvh;
};
#endif
