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
    const auto& phy = coordinator->GetComponent<Physics>(e);
    //only check objects that are moving
     if(phy.velocity != glm::vec3(0.0f,0.0f,0.0f)) {
      m_bvh.Update(e);
      toCheck.push_back(e);
     }
  }
  
  for(auto en : toCheck) {
    std::forward_list<Entity> overlaps = m_bvh.QueryOverlaps(en);
    auto& phy = coordinator->GetComponent<Physics>(en);
    auto& c = coordinator->GetComponent<Collidable>(en);
    auto& t = coordinator->GetComponent<Transform>(en);
    for(auto& e : overlaps) {

      auto& c2 = coordinator->GetComponent<Collidable>(e);
      auto& phy2 = coordinator->GetComponent<Physics>(e);
      auto& t2 = coordinator->GetComponent<Transform>(e);
      phy2.velocity += phy.velocity * glm::vec3(c2.length, c2.height, c2.width);
      
      phy.force = glm::vec3(0.0f,0.0f,0.0f);
      phy.acceleration = glm::vec3(0.0f,0.0f,0.0f);
      //normalize velocity vector, * by extents of collidable
      //t2.position += -glm::normalize(phy.velocity) * glm::vec3(c2.length*2,c2.height*2,c2.width*2);
      phy.velocity = glm::vec3(0.0f,0.0f,0.0f);
      m_bvh.Update(e);

	
      //      coordinator->RemoveComponent<Texture>(e);
      //coordinator->AddComponent<Texture>(e, Texture { "cube", 20.0f });
    }


    }
}
