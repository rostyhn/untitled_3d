#ifndef BVH_H
#define BVH_H

#include "../Components/Collidable.h"
#include "../Components/Transform.h"
#include "Coordinator.h"
#include "Maths.h"

#include "TypeDefinitions.h"
#include <glm/glm.hpp>
#include <stdlib.h>

// make this an observer or something in the future for m_entities
struct ColliderPair {
  ColliderPair(Entity first, Entity second) {
    e1 = first;
    e2 = second;
  }
  Entity e1, e2;
};

struct Node {
  Node() : m_parent(nullptr){
    m_children[0] = nullptr;
    m_children[1] = nullptr;
  }

  // leaf constructor
  Node(const Entity e) {
    Coordinator *c = &Coordinator::GetInstance();
    const auto &cl = c->GetComponent<Collidable>(e);
    m_minX = cl.corners[0][0];
    m_minY = cl.corners[0][1];
    m_minZ = cl.corners[0][2];

    m_maxX = cl.corners[7][0];
    m_maxY = cl.corners[7][1];
    m_maxZ = cl.corners[7][2];

    m_entity = e;
    m_children[0] = nullptr;
    m_children[1] = nullptr;
  }

  bool IsLeaf() const { return m_children[0] == nullptr; }

  void SetBranch(Node *a, Node *b) {
    a->m_parent = this;
    b->m_parent = this;

    m_children[0] = a;
    m_children[1] = b;
  }

  void SetLeaf(Entity e) {
    m_entity = e;
    m_children[0] = nullptr;
    m_children[1] = nullptr;
  }

  Node(Node *left, Node *right) {
    m_children[0] = left;
    m_children[1] = right;

    m_minX = std::min(left->m_minX, right->m_minX);
    m_minY = std::min(left->m_minY, right->m_minY);
    m_minZ = std::min(left->m_minZ, right->m_minZ);
    m_maxX = std::max(left->m_maxX, right->m_maxX);
    m_maxY = std::max(left->m_maxY, right->m_maxY);
    m_maxZ = std::max(left->m_maxZ, right->m_maxZ);
  }

  Node Union(Node *n) {
    Node newNode = Node { n, this };
    return newNode;
  }

  float Volume() {
    return abs((m_maxX - m_minX) * (m_maxY - m_minY) * (m_maxZ - m_minZ));
  }

  // basically these guys are the "fake" bBox values
  // the ones that are moved by the translations when needed
  // thats why we store a reference to the real thing as well as these ones

  void CalculateBoundingBox(float margin) {

    if (IsLeaf()) {
      Coordinator *coordinator = &Coordinator::GetInstance();

      const auto &transform = coordinator->GetComponent<Transform>(m_entity);
      const auto &bBox = coordinator->GetComponent<Collidable>(m_entity);

      glm::mat4 trans = Maths::CreateTransformMatrix(
          transform.position, transform.rotation, transform.scale);

      std::vector<float> newBox;

      if (transform.rotation == glm::vec3(0.0f, 0.0f, 0.0f)) {

        glm::vec4 newMin = trans * glm::vec4(bBox.corners[0], 1.0f);
        glm::vec4 newMax = trans * glm::vec4(bBox.corners[7], 1.0f);

	m_minX = newMin[0] - margin;
        m_minY = newMin[1] - margin;
        m_minZ = newMin[2] - margin;
        m_maxX = newMax[0] + margin;
        m_maxY = newMax[1] + margin;
        m_maxZ = newMax[2] + margin;

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
	
        m_minX = minX - margin;
        m_minY = minY - margin;
        m_minZ = minZ - margin;
        m_maxX = maxX + margin;
        m_maxY = maxY + margin;
        m_maxZ = maxZ + margin;
      }
    } else {
        m_children[0]->CalculateBoundingBox(margin);
        m_children[1]->CalculateBoundingBox(margin);

        m_minX = std::min(m_children[0]->m_minX, m_children[1]->m_minX);
        m_minY = std::min(m_children[0]->m_minY, m_children[1]->m_minY);
        m_minZ = std::min(m_children[0]->m_minZ, m_children[1]->m_minZ);
        m_maxX = std::max(m_children[0]->m_maxX, m_children[1]->m_maxX);
        m_maxY = std::max(m_children[0]->m_maxY, m_children[1]->m_maxY);
        m_maxZ = std::max(m_children[0]->m_maxZ, m_children[1]->m_maxZ);
    }
  }

  Node *GetSibling() const {
    return this == m_parent->m_children[0] ? m_parent->m_children[1]
                                           : m_parent->m_children[0];
  }

  float m_minX, m_minY, m_minZ;
  float m_maxX, m_maxY, m_maxZ;
  Node *m_children[2];
  Node *m_parent;
  Entity m_entity;
  bool m_childrenCrossed = false;
};

class BVH {
public:
  BVH() : m_root(nullptr) {}
  void Add(Entity e);
  void Remove();
  void Update();
  void InsertNode(Node *node, Node **parent);
  std::vector<ColliderPair> ComputePairs();
  void ClearChildrenCrossedFlag(Node *node);
  void CrossChildren(Node *node);
  void ComputePairsHelper(Node *n0, Node *n1);
  void UpdateNodeHelper(Node *node);
  std::set<Entity> m_Entities;
  void RemoveNode(Node* node);

  // removal methods
  // ray intersection
private:
  Node *m_root;
  const float m_margin = 0.2f;
  std::vector<Node *> m_invalidNodes;
  std::vector<ColliderPair> m_cp;
};
#endif
