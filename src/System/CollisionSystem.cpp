#include "CollisionSystem.h"

#include "../Components/Texture.h"
#include "../Components/AABB.h"
#include "../Toolbox/Coordinator.h"
#include "../Toolbox/Maths.h"


void CollisionSystem::Update() {

  Coordinator* coordinator = &Coordinator::GetInstance();

  //next step is to sort this into some sort of structure
  
  for(auto const& e : m_Entities) {
    const auto& collider1 = coordinator->GetComponent<Collidable>(e);

      auto& transform = coordinator->GetComponent<Transform>(e);
      std::vector<float> a = ReturnExtents(e, collider1.collidableType, transform);
      for(auto const& e2: m_Entities) {
	if(e != e2) {
	  const auto& collider2 = coordinator->GetComponent<Collidable>(e2);
	  const auto& transform2 = coordinator->GetComponent<Transform>(e2);
	  std::vector<float> b = ReturnExtents(e2, collider2.collidableType, transform2);
	  if((a[0] <= b[3] && a[3] >= b[0]) &&
	     (a[1] <= b[4] && a[4] >= b[1]) &&
	     (a[2] <= b[5] && a[5] >= b[2])) {
	    coordinator->RemoveComponent<Texture>(e2);
	    coordinator->AddComponent(e2, Texture { "cube", 1.0f });
	    //transform.position -= glm::vec3(-1.0f,-1.0f,-1.0f);
	  } else {
	  coordinator->RemoveComponent<Texture>(e2);
	  coordinator->AddComponent(e2, Texture { "box", 1.0f });
	  }
	}
    }
  }
}

std::vector<float> CollisionSystem::ReturnExtents(const Entity e, const CollidableType ct, const Transform transform) {
  switch(ct) {
  case BoundingBox:
  default:
    Coordinator* coordinator = &Coordinator::GetInstance();
    const auto& aabb = coordinator->GetComponent<AABB>(e); 
    return CalculateBoundingBox(aabb, transform);
    break;
  }
}

std::vector<float> CollisionSystem::CalculateBoundingBox(const AABB bBox, const Transform transform) {
  
  glm::mat4 trans = Maths::CreateTransformMatrix(transform.position, transform.rotation, transform.scale);

  std::vector<float> newBox;
  
  if(transform.rotation == glm::vec3(0.0f,0.0f,0.0f)) {
  
    glm::vec4 newMin = trans * glm::vec4(bBox.corners[0], 1.0f);
    glm::vec4 newMax = trans * glm::vec4(bBox.corners[7], 1.0f);

    newBox.push_back(newMin[0]);
    newBox.push_back(newMin[1]);
    newBox.push_back(newMin[2]);

    newBox.push_back(newMax[0]);
    newBox.push_back(newMax[1]);
    newBox.push_back(newMax[2]);
    
  } else {

    float minX, minY, minZ = std::numeric_limits<float>::max();
    float maxX, maxY, maxZ = std::numeric_limits<float>::min();
    
    for(int i = 0; i < 8; i++) {
      glm::vec4 newVec = trans * glm::vec4(bBox.corners[i], 1.0f);

      if(newVec[0] < minX) {
	minX = newVec[0];
      }

      if(newVec[0] > maxX) {
	maxX = newVec[0];
      }

      if(newVec[1] < minY) {
	minY = newVec[1];
      }

      if(newVec[1] > maxY) {
	maxY = newVec[1];
      }

      if(newVec[2] < minZ) {
	minZ = newVec[2];
      }
      
      if(newVec[2] > maxZ) {
	maxZ = newVec[2];
      }  
    }
    newBox.push_back(minX);
    newBox.push_back(minY);
    newBox.push_back(minZ);
    newBox.push_back(maxX);
    newBox.push_back(maxY);
    newBox.push_back(maxZ);
  }
  
  return newBox;
}
