#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"

class PhysicsSystem : public System {
public:
 PhysicsSystem() {}
 void Update(const float t, const float dt);
private:
  //allegedly -9.8 is too low for games
 const float gravity = -9.8f;
};
#endif
