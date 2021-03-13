#ifndef BVHNODE_H
#define BVHNODE_H

#include "Coordinator.h"
#include "../Components/Collidable.h"
#include "../Components/Transform.h"
#include "../Components/AABB.h"

#include <stdlib.h>

#define AABB_NULL_NODE 0xffffffff

struct BVHNode {
BVHNode() : m_parentNodeIndex(AABB_NULL_NODE), m_leftNodeIndex(AABB_NULL_NODE), m_rightNodeIndex(AABB_NULL_NODE), m_nextNodeIndex(AABB_NULL_NODE) {

}
  void BindEntity(const Entity e) {
    Coordinator *c = &Coordinator::GetInstance();
    const auto &cl = c->GetComponent<Collidable>(e);
    aabb = {  cl.aabb }; 
    m_entity = e;
  }

  void CalculateBoundingBox(const float margin, const Entity e) {
       Coordinator *coordinator = &Coordinator::GetInstance();

      //pg 87, Real-time Collision Detection / Christer Ericson
      const auto &transform = coordinator->GetComponent<Transform>(e);
      const auto &bBox = coordinator->GetComponent<Collidable>(e);
      glm::mat4 rotMat = Maths::CreateRotationMatrix(transform.rotation);
      
      for(int i = 0; i < 3; i++) {

	//scale the bounding box first, then move it
	aabb.m_min[i] = (bBox.aabb.m_min[i] * transform.scale[i]) + transform.position[i];
	aabb.m_max[i] = (bBox.aabb.m_max[i] * transform.scale[i]) + transform.position[i];
	
	for(int j = 0; j < 3; j++) {
	  float e = rotMat[i][j] * aabb.m_min[j];
	  float f = rotMat[i][j] * aabb.m_max[j];

	  if(e < f) {
	    aabb.m_min[i] += e;
	    aabb.m_max[i] += f;
	  } else {
	    aabb.m_min[i] += f;
	    aabb.m_max[i] += e;
	  } 
	}
      }
  }
  
  bool IsLeaf() const { return m_leftNodeIndex == AABB_NULL_NODE; }
  
  AABB aabb;
  unsigned m_parentNodeIndex, m_leftNodeIndex, m_rightNodeIndex, m_nextNodeIndex;
  Entity m_entity;
};
#endif
