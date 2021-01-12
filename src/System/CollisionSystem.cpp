#include "CollisionSystem.h"

#include "../Components/Texture.h"
#include "../Components/Collidable.h"
#include "../Components/Physics.h"
#include "../Toolbox/Coordinator.h"
#include "../Toolbox/Maths.h"

#include <forward_list>
#include <algorithm>

CollisionSystem::CollisionSystem() : m_bvh(BVH(1500)) {
  
}

void CollisionSystem::Update() {

  Coordinator* coordinator = &Coordinator::GetInstance();

  //wish there was a notify function or something
  if(m_Entities.size() > m_bvh.m_Entities.size()) {
    //find which entities are missing and add
    std::vector<Entity> diff;
    std::set_difference(m_Entities.begin(), m_Entities.end(), m_bvh.m_Entities.begin(), m_bvh.m_Entities.end(),
			std::inserter(diff, diff.begin()));
    for(Entity e : diff) {
      m_bvh.Add(e);
    }
  }

  std::vector<Entity> toCheck;
  for(const auto& e : m_Entities) {
    const auto& collidable = coordinator->GetComponent<Collidable>(e);
    const auto& phy = coordinator->GetComponent<Physics>(e);
    if(!collidable.isStatic) {
      m_bvh.Update(e);
      toCheck.push_back(e);
      }
    }
  
  for(auto en : toCheck) {
    std::forward_list<Entity> overlaps = m_bvh.QueryOverlaps(en);
    for(auto e : overlaps) {
      //  auto& phy = coordinator->GetComponent<Physics>(e);
      //phy.acceleration = glm::vec3(0.0f,0.0f,0.0f);
      //phy.velocity = glm::vec3(0.0f,0.0f,0.0f);
      coordinator->RemoveComponent<Texture>(e);
      coordinator->AddComponent<Texture>(e, Texture { "cube", 20.0f });
      
      //std::cout << "collision detected" << std::endl;
      
    }
  }
}
