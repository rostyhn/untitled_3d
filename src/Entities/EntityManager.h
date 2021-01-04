//https://austinmorlan.com/posts/entity_component_system/
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <queue>
#include <array>


//go back and possibly make this into a singleton

class EntityManager {
 public:
  EntityManager() {
    for(Entity entity = 0; entity < MAX_ENTITIES; entity++) {
      //init all values
      m_unusedEntities.push(entity);
    }
  }
  Entity CreateEntity() {
    //adds a new entity to the list
    Entity id = m_unusedEntities.front();
    m_unusedEntities.pop();
    m_currentEntityCount++;
    return id;   
  }
  
  void DestroyEntity(Entity entity) {
    //deletes entity from list

    m_signatures[entity].reset(); //resets the bitmask

    m_unusedEntities.push(entity);
    m_currentEntityCount--;
  }

  void setSignature(Entity entity, Signature sig) {
    //sets what components are enabled for an entity

    m_signatures[entity] = sig;
  }

  Signature getSignature(Entity entity) {
    return m_signatures[entity];
  }
  
 private:
  std::queue<Entity> m_unusedEntities{};
  // array of signatures where the index = to the id of the entity who has them
  std::array<Signature, MAX_ENTITIES> m_signatures{};

  uint32_t m_currentEntityCount{};
  
};


#endif 

