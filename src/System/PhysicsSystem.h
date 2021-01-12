#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"

class PhysicsSystem : public System {
public:
 PhysicsSystem() {}
 void Update(const double t, const double dt);
private:
 const float gravity = -9.8f;
  const float friction = 0.4;
};
#endif
