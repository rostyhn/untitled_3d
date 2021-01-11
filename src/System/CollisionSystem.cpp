#include "CollisionSystem.h"

#include "../Components/Texture.h"
#include "../Toolbox/Coordinator.h"
#include "../Toolbox/Maths.h"

#include <algorithm>

CollisionSystem::CollisionSystem() : m_bvh(BVH()) {
  
}

void CollisionSystem::Update() {

  Coordinator* coordinator = &Coordinator::GetInstance();

  //next step is to sort this into some sort of structure
  if(m_Entities.size() > m_bvh.m_Entities.size()) {
    std::cout << "added entities" << std::endl;
    //find which entities are missing and add
    std::vector<Entity> diff;
    std::set_difference(m_Entities.begin(), m_Entities.end(), m_bvh.m_Entities.begin(), m_bvh.m_Entities.end(),
			std::inserter(diff, diff.begin()));
    for(Entity e : diff) {
      m_bvh.Add(e);
    }
  }
  
  m_bvh.Update();
  std::vector<ColliderPair> cp = m_bvh.ComputePairs();
  for(ColliderPair c : cp) {
      Coordinator* coordinator = &Coordinator::GetInstance();

      /*      coordinator->RemoveComponent<Texture>(c.e1);
      coordinator->AddComponent(c.e1, Texture { "cube", 1.0f });  
      coordinator->RemoveComponent<Texture>(c.e2);
      coordinator->AddComponent(c.e2, Texture { "cube", 1.0f });  */

      auto& t1 = coordinator->GetComponent<Transform>(c.e1);
      auto& t2 = coordinator->GetComponent<Transform>(c.e2);
      glm::vec3 d = glm::vec3(-1.0f,-1.0f,-1.0f);
      t1.position += d;
      t2.position += d;
  }

}
