#ifndef AABB_H
#define AABB_H

#include "../Toolbox/Coordinator.h"
#include "../Components/Transform.h"
#include "../Toolbox/Maths.h"

#include <glm/gtx/transform.hpp>

struct AABB {
  
  //make it faster with balancing
AABB() : m_surfaceArea(0.0f) {
  m_length = 0.0f;
  m_height = 0.0f;
  m_width  = 0.0f;
}

AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
  m_min[0] = minX;
  m_min[1] = minY;
  m_min[2] = minZ;

  m_max[0] = maxX;
  m_max[1] = maxY;
  m_max[2] = maxZ;
  
  m_surfaceArea = SurfaceArea();

  m_length = m_max[0] - m_min[0];
  m_height = m_max[1] - m_min[1];
  m_width  = m_max[2] - m_min[2];
}
  
  AABB Merge(AABB* n) {
    AABB newNode = AABB { };
    
    newNode.m_min[0] = std::min(this->m_min[0], n->m_min[0]);
    newNode.m_min[1] = std::min(this->m_min[1], n->m_min[1]);
    newNode.m_min[2] = std::min(this->m_min[2], n->m_min[2]);

    newNode.m_max[0] = std::max(this->m_max[0], n->m_max[0]);
    newNode.m_max[1] = std::max(this->m_max[1], n->m_max[1]);
    newNode.m_max[2] = std::max(this->m_max[2], n->m_max[2]);

    newNode.m_surfaceArea = newNode.SurfaceArea();
    
    return newNode;
  }
			     
  float SurfaceArea() {
    //dx * dy + dy*dz + dz*dx
    return 2.0f * (m_max[0] - m_min[0]) * (m_max[1] - m_min[1]) + (m_max[1] - m_min[1]) * (m_max[2] - m_min[2]) + (m_max[2] - m_min[2]) * (m_max[0] - m_min[0]);
  }

  
   bool Overlaps(AABB& other) {
    return m_max[0] > other.m_min[0] &&
	   m_min[0] < other.m_max[0] &&
	   m_max[1] > other.m_min[1] &&
	   m_min[1] < other.m_max[1] &&
	   m_max[2] > other.m_min[2] &&
	   m_min[2] < other.m_max[2];
  }

  bool Contains(AABB& other) {
    return other.m_min[0] >= m_min[0] &&
           other.m_max[0] <= m_max[0] &&
	   other.m_min[1] >= m_min[1] &&
	   other.m_max[1] <= m_max[1] &&
	   other.m_min[2] >= m_min[2] &&
	   other.m_max[2] <= m_max[2];
  }

  float m_min[3] = {0.0f};
  float m_max[3] = {0.0f};            ;
  float m_surfaceArea, m_width, m_length, m_height;
};
#endif
