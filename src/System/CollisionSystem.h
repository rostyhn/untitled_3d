#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../Components/Collidable.h"
#include "../Components/Transform.h"
#include "System.h"
#include <vector>

class CollisionSystem : public System {

public:
  CollisionSystem() {}
  void Update();
private:
  std::vector<float> CalculateBoundingBox(Collidable bBox, Transform transform);
};
#endif
