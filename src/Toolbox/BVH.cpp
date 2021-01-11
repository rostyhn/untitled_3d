#include "BVH.h"
#include "../Components/Texture.h"

void BVH::UpdateNodeHelper(Node *node) {

  
  if (node->IsLeaf()) {
    Node moved = Node{node->m_entity};
    moved.CalculateBoundingBox(0.0f);
    if ((moved.m_minX < node->m_minX || moved.m_minY < node->m_minY ||
         moved.m_minZ < node->m_minZ) ||
        (moved.m_maxX > node->m_maxX || moved.m_maxY > node->m_maxY ||
         moved.m_maxZ > node->m_maxZ)) {
      m_invalidNodes.push_back(node);
    }
  } else {
    UpdateNodeHelper(node->m_children[0]);
    UpdateNodeHelper(node->m_children[1]);
  }
}

void BVH::Add(Entity e) {
  if (m_root) {
    Node *node = new Node(e);
    node->SetLeaf(e);
    node->CalculateBoundingBox(m_margin);
    InsertNode(node, &m_root);
  } else {
    m_root = new Node(e);
    m_root->SetLeaf(e);
    m_root->CalculateBoundingBox(m_margin);
    m_root->m_parent = nullptr;
  }
  m_Entities.insert(e);
}

void BVH::InsertNode(Node *node, Node **parent) {
  Node *p = *parent;
  if (p->IsLeaf()) {
    Node *newParent = new Node();
    newParent->m_parent = p->m_parent;
    newParent->SetBranch(node, p);
    *parent = newParent;
  } else {

    float volDiff0 =
        p->m_children[0]->Union(node).Volume() - p->m_children[0]->Volume();
    float volDiff1 =
        p->m_children[1]->Union(node).Volume() - p->m_children[1]->Volume();

    if (volDiff0 < volDiff1) {
      InsertNode(node, &p->m_children[0]);
    } else {
      InsertNode(node, &p->m_children[1]);
    }
  }
  (*parent)->CalculateBoundingBox(m_margin);
}

/*void BVH::Remove(Entity e) {

  }*/

void BVH::RemoveNode(Node *node) {
  Node *parent = node->m_parent;
  if (parent) {
    Node *sibling = node->GetSibling();
    if (parent->m_parent) {
      sibling->m_parent = parent->m_parent;
      (parent == parent->m_parent->m_children[0]
           ? parent->m_parent->m_children[0]
           : parent->m_parent->m_children[1]) = sibling;
    } else {
      Node *sib = node->GetSibling();
      m_root = sib;
      sib->m_parent = nullptr;
    }
  } else {
    m_root = nullptr;
    delete node;
  }
}

std::vector<ColliderPair> BVH::ComputePairs() {
  m_cp.clear();
 
  if (!m_root || m_root->IsLeaf())
    return m_cp;
  //switch to using a stack
  ClearChildrenCrossedFlag(m_root);
  ComputePairsHelper(m_root->m_children[0], m_root->m_children[1]);
  return m_cp;
}

void BVH::ClearChildrenCrossedFlag(Node *node) {
  node->m_childrenCrossed = false;
  if (!node->IsLeaf()) {
    ClearChildrenCrossedFlag(node->m_children[0]);
    ClearChildrenCrossedFlag(node->m_children[1]);
  }
}

void BVH::CrossChildren(Node *node) {
  if (!node->m_childrenCrossed) {
    ComputePairsHelper(node->m_children[0], node->m_children[1]);
    node->m_childrenCrossed = true;
  }
}

void BVH::ComputePairsHelper(Node *n0, Node *n1) {
  if (n0->IsLeaf()) {
    if (n1->IsLeaf()) {
      if ((n0->m_minX <= n1->m_maxX && n0->m_maxX >= n1->m_minX) &&
          (n0->m_minY <= n1->m_maxY && n0->m_maxY >= n1->m_minY) &&
          (n0->m_minZ <= n1->m_maxZ && n0->m_maxZ >= n1->m_minZ)) {
        m_cp.push_back(ColliderPair{n0->m_entity, n1->m_entity});
      }
    } else {
      CrossChildren(n1);
      ComputePairsHelper(n0, n1->m_children[0]);
      ComputePairsHelper(n0, n1->m_children[1]);
    }
  } else {
    if (n1->IsLeaf()) {
      CrossChildren(n0);
      ComputePairsHelper(n0->m_children[0], n1);
      ComputePairsHelper(n0->m_children[1], n1);
    } else {
      CrossChildren(n0);
      CrossChildren(n1);
      ComputePairsHelper(n0->m_children[0], n1->m_children[0]);
      ComputePairsHelper(n0->m_children[0], n1->m_children[1]);
      ComputePairsHelper(n0->m_children[1], n1->m_children[0]);
      ComputePairsHelper(n0->m_children[1], n1->m_children[1]);
    }
  }
}

void BVH::Update() {
  if (m_root) {
    if (m_root->IsLeaf()) {
      m_root->CalculateBoundingBox(m_margin);
    } else {
      m_invalidNodes.clear();
      UpdateNodeHelper(m_root);

      for (Node *node : m_invalidNodes) {
        Node *parent = node->m_parent;
        Node *sibling = node->GetSibling();
	Node **parentLink = parent->m_parent
            ? (parent == parent->m_parent->m_children[0]
                ? &parent->m_parent->m_children[0]
                : &parent->m_parent->m_children[1])
            : &m_root;
        // replace parent with sibling
        sibling->m_parent =
          parent->m_parent
            ? parent->m_parent
            : nullptr; // root has null parent

        *parentLink = sibling;
        delete parent;
        
        // re-insert node
        node->CalculateBoundingBox(m_margin);
        InsertNode(node, &m_root);
      }
      m_invalidNodes.clear();
    }
  }
}
