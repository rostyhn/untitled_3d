#include "PhysicsSystem.h"
#include "../Toolbox/Coordinator.h"

#include "../Components/Transform.h"
#include "../Components/Physics.h"
#include "../Components/Collidable.h"

#include <math.h>

void PhysicsSystem::Update(const float t, const float dt) {

  Coordinator* coordinator = &Coordinator::GetInstance();
  for(const auto& e : m_Entities) {
    
    auto& t = coordinator->GetComponent<Transform>(e);
    auto& p = coordinator->GetComponent<Physics>(e);
    auto& c = coordinator->GetComponent<Collidable>(e);

    //super basic physics for now, just gravity

    t.position += p.velocity * dt;
    p.acceleration += p.forceAccum * p.invMass;
    p.velocity += p.acceleration * dt;
    p.velocity *= powf(p.damping,dt);

    p.ClearAccumulator();
    
    if(t.position[1] < c.aabb.m_height) {
      t.position = glm::vec3(t.position[0],c.aabb.m_height,t.position[2]);
      
    } else {
      if(p.invMass != 0.0f) {
	p.AddForce(glm::vec3(0.0f,gravity / p.invMass,0.0f));
      }
    }
    
  }
}
