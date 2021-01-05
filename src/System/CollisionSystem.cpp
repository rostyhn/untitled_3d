#include "CollisionSystem.h"

#include "../Toolbox/Coordinator.h"

void CollisionSystem::Update() {

  Coordinator* coordinator = &Coordinator::GetInstance();

  //next step is to sort this into some sort of structure

  
  for(auto const& e : m_Entities) {
    const auto& bBox = coordinator->GetComponent<Collidable>(e);
    auto& transform = coordinator->GetComponent<Transform>(e);
    std::vector<float> a = CalculateBoundingBox(bBox, transform);
    for(auto const& e2: m_Entities) {
      if(e != e2) {
	const auto& bBox2 = coordinator->GetComponent<Collidable>(e2);
	const auto& transform2 = coordinator->GetComponent<Transform>(e2);
	std::vector<float> b = CalculateBoundingBox(bBox2, transform2);
	if((a[0] <= b[3] && a[3] >= a[0]) &&
	   (a[1] <= b[4] && a[4] >= b[1]) &&
	   (a[2] <= b[5] && a[5] >= b[5]))
	  {
	    //coordinator->RemoveComponent<Texture>(e2);
	    //coordinator->AddComponent(e2, Texture { "cube", 1.0f });
	    //transform.position -= glm::vec3(-1.0f,-1.0f,-1.0f);
	  }
      }
    }
  }
}

std::vector<float> CollisionSystem::CalculateBoundingBox(Collidable bBox, Transform transform) {
  std::vector<float> newBox {
    bBox.minX * transform.scale[0] + transform.position[0], //0
    bBox.minY * transform.scale[1] + transform.position[1], //1 
    bBox.minZ * transform.scale[2] + transform.position[2], //2 
    bBox.maxX * transform.scale[0] + transform.position[0], //3 
    bBox.maxY * transform.scale[1] + transform.position[1], //4 
    bBox.maxZ * transform.scale[2] + transform.position[2], //5 
      };
  return newBox;
}
