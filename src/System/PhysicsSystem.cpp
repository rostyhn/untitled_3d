#include "PhysicsSystem.h"
#include "../Toolbox/Coordinator.h"

#include "../Components/Transform.h"
#include "../Components/Physics.h"

void PhysicsSystem::Update(const double t, const double dt) {

  Coordinator* coordinator = &Coordinator::GetInstance();
  for(const auto& e : m_Entities) {
    auto& trans = coordinator->GetComponent<Transform>(e);
    auto& physics = coordinator->GetComponent<Physics>(e);
    //physics.acceleration += (glm::vec3(0.0f,gravity,0.0f) * glm::vec3(dt,dt,dt));
    physics.velocity += (glm::vec3(0.0f,gravity,0.0f) * glm::vec3(dt,dt,dt));
    trans.position += (physics.velocity * glm::vec3(dt,dt,dt));
  }
}
