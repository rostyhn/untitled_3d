#include "PhysicsSystem.h"
#include "../Toolbox/Coordinator.h"

#include "../Components/Transform.h"
#include "../Components/Physics.h"
#include "../Components/Collidable.h"

void PhysicsSystem::Update(const double t, const double dt) {

  Coordinator* coordinator = &Coordinator::GetInstance();
  for(const auto& e : m_Entities) {
    auto& trans = coordinator->GetComponent<Transform>(e);
    auto& physics = coordinator->GetComponent<Physics>(e);
    auto& c = coordinator->GetComponent<Collidable>(e);
    
    glm::vec3 force = glm::vec3(physics.force[0],gravity*physics.mass, physics.force[2]);

    physics.acceleration = glm::vec3(force[0] / physics.mass, force[1] / physics.mass,
				     force[2] / physics.mass);
    physics.velocity += (physics.acceleration * glm::vec3(dt,dt,dt));
    trans.position += (physics.velocity * glm::vec3(dt,dt,dt));


    //this is the bottleneck, need to figure out a better way for the blocks to hit the floor
    if(trans.position[1] < c.height) {
    	physics.acceleration = glm::vec3(0.0f,0.0f,0.0f);
	physics.velocity = glm::vec3(physics.velocity[0],0.0f,physics.velocity[2]);
    	trans.position = glm::vec3(trans.position[0], c.height, trans.position[2]);
    }
    
  }
}
