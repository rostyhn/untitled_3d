#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>

struct Physics {
  glm::vec3 velocity;
  glm::vec3 acceleration;
  float mass;
};
#endif
