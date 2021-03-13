#include "CollisionSystem.h"

#include "../Components/Texture.h"
#include "../Components/Collidable.h"
#include "../Components/Physics.h"
#include "../Toolbox/Coordinator.h"
#include "../Toolbox/Maths.h"

#include <forward_list>
#include <algorithm>

CollisionSystem::CollisionSystem() : m_bvh(BVH(1500, 1.0f)) {
  
}

void CollisionSystem::Update() {

  Coordinator* coordinator = &Coordinator::GetInstance();
  
  std::vector<Entity> toCheck;
  for(const auto& e : m_Entities) {
    const auto& phy = coordinator->GetComponent<Physics>(e);
    const auto& c = coordinator->GetComponent<Collidable>(e);
    //only check objects that are moving
    if(!c.isStatic) {
      m_bvh.Update(e);
      toCheck.push_back(e);
     }
  }

  
  for(auto en : toCheck) {
    std::forward_list<Entity> overlaps = m_bvh.QueryOverlaps(en);
    auto& c = coordinator->GetComponent<Collidable>(en);
    auto& t = coordinator->GetComponent<Transform>(en);    
    for(auto& e : overlaps) {      
      //narrowphase collision detection; for now let's just keep anything that collided seperate
      auto& c2 = coordinator->GetComponent<Collidable>(e);
      auto& t2 = coordinator->GetComponent<Transform>(e);
      //so you need the model, perhaps there should be a way to get right into the loader, grab the points and run the support function on it
      
      
    }							   
    
    }
}

void CollisionSystem::NotifyAdded(const Entity e) {
  m_bvh.Add(e);
}

void CollisionSystem::NotifyErased(const Entity e) {
  //  m_bvh.Remove(e);
}
