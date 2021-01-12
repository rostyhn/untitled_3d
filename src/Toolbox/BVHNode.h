#ifndef BVHNODE_H
#define BVHNODE_H

#include "Coordinator.h"
#include "../Components/Collidable.h"
#include "../Components/Transform.h"
#include "Maths.h"

#include <stdlib.h>

#define AABB_NULL_NODE 0xffffffff

struct AABB {


  AABB() : m_minX(0.0f), m_minY(0.0f), m_minZ(0.0f), m_maxX(0.0f), m_maxY(0.0f), m_maxZ(0.0f), m_surfaceArea(0.0f) { }

AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) :
		m_minX(minX), m_minY(minY), m_minZ(minZ), m_maxX(maxX), m_maxY(maxY), m_maxZ(maxZ) {
		m_surfaceArea = SurfaceArea();
}
  
  AABB Merge(AABB* n) {
    AABB newNode = AABB { };

    newNode.m_minX = std::min(this->m_minX, n->m_minX);
    newNode.m_minY = std::min(this->m_minY, n->m_minY);
    newNode.m_minZ = std::min(this->m_minZ, n->m_minZ);

    newNode.m_maxX = std::max(this->m_maxX, n->m_maxX);
    newNode.m_maxY = std::max(this->m_maxY, n->m_maxY);
    newNode.m_maxZ = std::max(this->m_maxZ, n->m_maxZ);
    newNode.m_surfaceArea = newNode.SurfaceArea();
      
    return newNode;
  }
			     
  float SurfaceArea() {
    //dx * dy + dy*dz + dz*dx
    return 2.0f * (m_maxX - m_minX) * (m_maxY - m_minY) + (m_maxY - m_minY) * (m_maxZ - m_minZ) + (m_maxZ-m_minZ) * (m_maxX - m_minX);
  }

  
   bool Overlaps(AABB& other) {
    return m_maxX > other.m_minX &&
	   m_minX < other.m_maxX &&
	   m_maxY > other.m_minY &&
	   m_minY < other.m_maxY &&
	   m_maxZ > other.m_minZ &&
	   m_minZ < other.m_maxZ;
  }

  bool Contains(AABB& other) {
    return other.m_minX >= m_minX &&
           other.m_maxX <= m_maxX &&
	   other.m_minY >= m_minY &&
	   other.m_maxY <= m_maxY &&
	   other.m_minZ >= m_minZ &&
	   other.m_maxZ <= m_maxZ;
  }

  float m_minX, m_minY, m_minZ;
  float m_maxX, m_maxY, m_maxZ;
  float m_surfaceArea;
};



struct BVHNode {
BVHNode() : m_entity(AABB_NULL_NODE), m_parentNodeIndex(AABB_NULL_NODE), m_leftNodeIndex(AABB_NULL_NODE), m_rightNodeIndex(AABB_NULL_NODE), m_nextNodeIndex(AABB_NULL_NODE) {

}
  void BindEntity(const Entity e) {
    Coordinator *c = &Coordinator::GetInstance();
    const auto &cl = c->GetComponent<Collidable>(e);
    aabb = {cl.corners[0][0], cl.corners[0][1], cl.corners[0][2], cl.corners[7][0], cl.corners[7][1], cl.corners[7][2] };
    m_entity = e;    
  }
  
  bool IsLeaf() const { return m_leftNodeIndex == AABB_NULL_NODE; }  

  
  void CalculateBoundingBox(float margin) {
      Coordinator *coordinator = &Coordinator::GetInstance();

      const auto &transform = coordinator->GetComponent<Transform>(m_entity);
      const auto &bBox = coordinator->GetComponent<Collidable>(m_entity);

      glm::mat4 trans = Maths::CreateTransformMatrix(
          transform.position, transform.rotation, transform.scale);

      std::vector<float> newBox;

      if (transform.rotation == glm::vec3(0.0f, 0.0f, 0.0f)) {

        glm::vec4 newMin = trans * glm::vec4(bBox.corners[0], 1.0f);
        glm::vec4 newMax = trans * glm::vec4(bBox.corners[7], 1.0f);

	aabb.m_minX = newMin[0] - margin;
        aabb.m_minY = newMin[1] - margin;
        aabb.m_minZ = newMin[2] - margin;
        aabb.m_maxX = newMax[0] + margin;
        aabb.m_maxY = newMax[1] + margin;
        aabb.m_maxZ = newMax[2] + margin;

      } else {

        float minX, minY, minZ = std::numeric_limits<float>::max();
        float maxX, maxY, maxZ = std::numeric_limits<float>::min();

        for (int i = 0; i < 8; i++) {
          glm::vec4 newVec = trans * glm::vec4(bBox.corners[i], 1.0f);

          if (newVec[0] < minX) {
            minX = newVec[0];
          }

          if (newVec[0] > maxX) {
            maxX = newVec[0];
          }

          if (newVec[1] < minY) {
            minY = newVec[1];
          }

          if (newVec[1] > maxY) {
            maxY = newVec[1];
          }

          if (newVec[2] < minZ) {
            minZ = newVec[2];
          }

          if (newVec[2] > maxZ) {
            maxZ = newVec[2];
          }
        }
	
        aabb.m_minX = minX - margin;
        aabb.m_minY = minY - margin;
        aabb.m_minZ = minZ - margin;
        aabb.m_maxX = maxX + margin;
        aabb.m_maxY = maxY + margin;
        aabb.m_maxZ = maxZ + margin;
      }    
  }
  AABB aabb;
  unsigned m_parentNodeIndex, m_leftNodeIndex, m_rightNodeIndex, m_nextNodeIndex;
  Entity m_entity;
};
#endif
