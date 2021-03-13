#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>

struct Physics {
  
  Physics() {}

  Physics(const float d, const float im) {
    damping = d;
    invMass = im;

    velocity = glm::vec3(0.0f,0.0f,0.0f);
    acceleration = glm::vec3(0.0f,0.0f,0.0f);
    forceAccum = glm::vec3(0.0f,0.0f,0.0f);
  }

  void AddForce(const glm::vec3 force) {
    forceAccum += force;
  }
  
  void ClearAccumulator() {
    forceAccum = glm::vec3(0.0f,0.0f,0.0f);
  }

  float damping;
  float invMass;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  glm::vec3 forceAccum;
};
#endif
