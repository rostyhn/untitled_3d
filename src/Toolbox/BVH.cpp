#include "BVH.h"
#include "../Components/Texture.h"
#include <stack>

#define AABB_NULL_NODE 0xffffffff

BVH::BVH(unsigned initalSize, float margin) : m_rootNodeIndex(AABB_NULL_NODE), m_allocatedNodeCount(0), m_nextFreeNodeIndex(0), m_nodeCapacity(initalSize), m_growthSize(initalSize), m_margin(margin) {
  m_nodes.resize(initalSize);
  for(unsigned ni = 0; ni < initalSize; ni++) {
    BVHNode& node = m_nodes[ni];
    node.m_nextNodeIndex = ni+1;
  }
  m_nodes[initalSize-1].m_nextNodeIndex = AABB_NULL_NODE;
}


BVH::~BVH() {
  m_nodes.clear();
  m_entityNodeMap.clear();
  m_Entities.clear();
}

unsigned BVH::AllocateNode() {

  if(m_nextFreeNodeIndex == AABB_NULL_NODE) {

    assert(m_allocatedNodeCount == m_nodeCapacity);

    m_nodeCapacity += m_growthSize;
    m_nodes.resize(m_nodeCapacity);
    
    for(unsigned ni = m_allocatedNodeCount; ni < m_nodeCapacity; ni++) {
      BVHNode& node = m_nodes[ni];
      node.m_nextNodeIndex = ni+1;
    }
    
    m_nodes[m_nodeCapacity -1].m_nextNodeIndex = AABB_NULL_NODE;
    m_nextFreeNodeIndex = m_allocatedNodeCount;
  }

  unsigned nodeIndex = m_nextFreeNodeIndex;
  BVHNode& newNode = m_nodes[nodeIndex];
  newNode.m_parentNodeIndex = AABB_NULL_NODE;
  newNode.m_leftNodeIndex = AABB_NULL_NODE;
  newNode.m_rightNodeIndex = AABB_NULL_NODE;
  m_nextFreeNodeIndex = newNode.m_nextNodeIndex;
  m_allocatedNodeCount++;

  return nodeIndex;
  
}

void BVH::DeallocateNode(unsigned nodeIndex) {
  BVHNode& deletedNode = m_nodes[nodeIndex];
  deletedNode.m_nextNodeIndex = m_nextFreeNodeIndex;
  m_nextFreeNodeIndex = nodeIndex;
  m_allocatedNodeCount--;
}

void BVH::Add(const Entity e) {
  unsigned nodeIndex = AllocateNode();
  BVHNode& node = m_nodes[nodeIndex];

  node.BindEntity(e);
  //make sure to have the bBox calculated in world coords
  node.CalculateBoundingBox(m_margin, e);
  
  InsertLeaf(nodeIndex);
  m_entityNodeMap[e] = nodeIndex;
  m_Entities.insert(e);
}

void BVH::Remove(const Entity e) {
  unsigned nodeIndex = m_entityNodeMap[e];
  RemoveLeaf(nodeIndex);
  DeallocateNode(nodeIndex);
  m_entityNodeMap.erase(e);
  m_Entities.erase(e);
}

void BVH::Update(const Entity e) {
  unsigned nodeIndex = m_entityNodeMap[e];
  UpdateLeaf(nodeIndex);
}


void BVH::InsertLeaf(unsigned leafNodeIndex) {
  
  assert(m_nodes[leafNodeIndex].m_parentNodeIndex == AABB_NULL_NODE);
  assert(m_nodes[leafNodeIndex].m_leftNodeIndex == AABB_NULL_NODE);
  assert(m_nodes[leafNodeIndex].m_rightNodeIndex == AABB_NULL_NODE);

  if(m_rootNodeIndex == AABB_NULL_NODE) {
    m_rootNodeIndex = leafNodeIndex;
    return;
  }

  unsigned treeNodeIndex = m_rootNodeIndex;
  BVHNode& leafNode = m_nodes[leafNodeIndex];
  
  while(!m_nodes[treeNodeIndex].IsLeaf()) {
    BVHNode& treeNode = m_nodes[treeNodeIndex];
    unsigned leftNodeIndex = treeNode.m_leftNodeIndex;
    unsigned rightNodeIndex = treeNode.m_rightNodeIndex;
    BVHNode& leftNode = m_nodes[leftNodeIndex];
    BVHNode& rightNode = m_nodes[rightNodeIndex];

    AABB combinedAABB = treeNode.aabb.Merge(&leafNode.aabb);
    
    float newParentCost = 2.0f * combinedAABB.m_surfaceArea;
    float minPushDownCost = 2.0f * (combinedAABB.m_surfaceArea - treeNode.aabb.m_surfaceArea);
    
    float costLeft;
    float costRight;
    
    if(leftNode.IsLeaf()) {
      costLeft = leafNode.aabb.Merge(&leftNode.aabb).m_surfaceArea + minPushDownCost;
    } else {
      AABB newLeftAABB = leafNode.aabb.Merge(&leftNode.aabb);
      costLeft = (newLeftAABB.m_surfaceArea - leftNode.aabb.m_surfaceArea) + minPushDownCost;
    }
    
    if(rightNode.IsLeaf()) {
      costRight = leafNode.aabb.Merge(&rightNode.aabb).m_surfaceArea + minPushDownCost;
    } else {
      AABB newRightAABB = leafNode.aabb.Merge(&rightNode.aabb);
      costRight = (newRightAABB.m_surfaceArea - rightNode.aabb.m_surfaceArea) + minPushDownCost;
    }

    if(newParentCost < costLeft && newParentCost < costRight) {
      break; //stupid way to get out but w/e
    }

    if(costLeft < costRight) {
      treeNodeIndex = leftNodeIndex;
    } else {
      treeNodeIndex = rightNodeIndex;
    }
  }

  unsigned leafSiblingIndex = treeNodeIndex;
  BVHNode& leafSibling = m_nodes[leafSiblingIndex];

  unsigned oldParentIndex = leafSibling.m_parentNodeIndex;
  unsigned newParentIndex = AllocateNode();

  BVHNode& newParent = m_nodes[newParentIndex];
  
  newParent.m_parentNodeIndex = oldParentIndex;
  newParent.aabb = leafNode.aabb.Merge(&leafSibling.aabb);

  newParent.m_leftNodeIndex = leafSiblingIndex;
  newParent.m_rightNodeIndex = leafNodeIndex;
  leafNode.m_parentNodeIndex = newParentIndex;
  leafSibling.m_parentNodeIndex = newParentIndex;

  if(oldParentIndex == AABB_NULL_NODE) {
    m_rootNodeIndex = newParentIndex;
  } else {
    BVHNode& oldParent = m_nodes[oldParentIndex];
    if(oldParent.m_leftNodeIndex == leafSiblingIndex) {
      oldParent.m_leftNodeIndex = newParentIndex;
    } else {
      oldParent.m_rightNodeIndex = newParentIndex;
    }
  }
  treeNodeIndex = leafNode.m_parentNodeIndex;
  FixUpwardsTree(treeNodeIndex);
}

void BVH::RemoveLeaf(unsigned leafNodeIndex) {

  if(leafNodeIndex == m_rootNodeIndex) {
    m_rootNodeIndex = AABB_NULL_NODE;
    return;
  }

  
  BVHNode& leafNode = m_nodes[leafNodeIndex];
  unsigned parentNodeIndex = leafNode.m_parentNodeIndex;
  BVHNode& parentNode = m_nodes[parentNodeIndex];
  unsigned grandParentNodeIndex = parentNode.m_parentNodeIndex;
  unsigned siblingNodeIndex = parentNode.m_leftNodeIndex == leafNodeIndex ? parentNode.m_rightNodeIndex : parentNode.m_leftNodeIndex;
  assert(siblingNodeIndex != AABB_NULL_NODE);
  BVHNode& siblingNode = m_nodes[siblingNodeIndex];
  
  if(grandParentNodeIndex != AABB_NULL_NODE) {

    BVHNode& grandParentNode = m_nodes[grandParentNodeIndex];
    if(grandParentNode.m_leftNodeIndex == parentNodeIndex) {
      grandParentNode.m_leftNodeIndex = siblingNodeIndex;
    } else {
      grandParentNode.m_rightNodeIndex = siblingNodeIndex;
    }
    siblingNode.m_parentNodeIndex = grandParentNodeIndex;
    DeallocateNode(parentNodeIndex);
    FixUpwardsTree(grandParentNodeIndex);
  } else {
    m_rootNodeIndex = siblingNodeIndex;
    siblingNode.m_parentNodeIndex = AABB_NULL_NODE;
    DeallocateNode(parentNodeIndex);
  }
  leafNode.m_parentNodeIndex = AABB_NULL_NODE;  
}

void BVH::UpdateLeaf(unsigned leafNodeIndex) {
  BVHNode& fattened = m_nodes[leafNodeIndex];
  BVHNode moved = BVHNode { };
  //possibly a bottleneck

  //think of making this better
  moved.BindEntity(fattened.m_entity);
  moved.CalculateBoundingBox(0.0f, fattened.m_entity);
  
  //check if fattened aabb contains moved
  if (fattened.aabb.Contains(moved.aabb)) return;

  
  RemoveLeaf(leafNodeIndex);
  
  fattened.aabb.m_min[0] = moved.aabb.m_min[0] - m_margin;
  fattened.aabb.m_min[1] = moved.aabb.m_min[1] - m_margin;
  fattened.aabb.m_min[2] = moved.aabb.m_min[2] - m_margin;
  fattened.aabb.m_max[0] = moved.aabb.m_max[0] + m_margin;
  fattened.aabb.m_max[1] = moved.aabb.m_max[1] + m_margin;
  fattened.aabb.m_max[2] = moved.aabb.m_max[2] + m_margin;
  fattened.aabb.m_surfaceArea = fattened.aabb.SurfaceArea();
  
  InsertLeaf(leafNodeIndex);
}

void BVH::FixUpwardsTree(unsigned treeNodeIndex) {
  while(treeNodeIndex != AABB_NULL_NODE) {
    BVHNode& treeNode = m_nodes[treeNodeIndex];

    assert(treeNode.m_leftNodeIndex != AABB_NULL_NODE && treeNode.m_rightNodeIndex != AABB_NULL_NODE);

    BVHNode& leftNode = m_nodes[treeNode.m_leftNodeIndex];
    BVHNode& rightNode = m_nodes[treeNode.m_rightNodeIndex];

    treeNode.aabb = leftNode.aabb.Merge(&rightNode.aabb);
    treeNode.aabb.m_surfaceArea = treeNode.aabb.SurfaceArea();

    //rotate(index);
    treeNodeIndex = treeNode.m_parentNodeIndex;    
  }
}

std::forward_list<Entity> BVH::QueryOverlaps(const Entity e) {
  std::forward_list<Entity> overlaps;
  std::stack<unsigned> stack;
  BVHNode& startNode = m_nodes[m_entityNodeMap[e]];
  stack.push(m_rootNodeIndex);
  
  while(!stack.empty()) {
    unsigned nodeIndex = stack.top();

    stack.pop();

    if(nodeIndex == AABB_NULL_NODE) continue;

    BVHNode& node = m_nodes[nodeIndex];
    if(node.aabb.Overlaps(startNode.aabb)) {
      if(node.IsLeaf() && node.m_entity != startNode.m_entity) {
	overlaps.push_front(node.m_entity);
      } else {
	stack.push(node.m_leftNodeIndex);
	stack.push(node.m_rightNodeIndex);
      }
    }
  }
  return overlaps;
}


