#ifndef BVH_H
#define BVH_H

#include "Coordinator.h"

#include "BVHNode.h"
#include "TypeDefinitions.h"
#include <glm/glm.hpp>
#include <stdlib.h>
#include <forward_list>
#include <set>

class BVH {
public:
  BVH(unsigned initalSize);
  ~BVH();
  void Add(const Entity e);
  void Remove(const Entity e);
  void Update(const Entity e);
  std::forward_list<Entity> QueryOverlaps(const Entity e);
  std::set<Entity> m_Entities;
  
private:
  //pointers are too slow
  unsigned m_rootNodeIndex;
  unsigned m_allocatedNodeCount;
  unsigned m_nextFreeNodeIndex;
  unsigned m_nodeCapacity;
  unsigned m_growthSize;
  
  unsigned AllocateNode();
  void DeallocateNode(unsigned nodeIndex);
  void InsertLeaf(unsigned leafNodeIndex);
  void RemoveLeaf(unsigned leafNodeIndex);
  void UpdateLeaf(unsigned leafNodeIndex);
  void FixUpwardsTree(unsigned treeNodeIndex);
  //map entities to their nodes
  std::vector<BVHNode> m_nodes;
  std::map<Entity, unsigned> m_entityNodeMap;

  const float m_margin = 1.0f;
};
#endif
