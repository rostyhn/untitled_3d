#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>

struct Physics {
  glm::vec3 velocity;
  glm::vec3 acceleration;
  glm::vec3 force;
  float mass;
};
#endif
