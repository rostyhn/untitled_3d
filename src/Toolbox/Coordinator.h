#ifndef COORDINATOR_H
#define COORDINATOR_H
#include "../Toolbox/TypeDefinitions.h"
#include "../Components/ComponentManager.h"
#include "../Entities/EntityManager.h"
#include "../System/SystemManager.h"

#include <iostream>

class Coordinator {
 public:
  void Init() {
    m_ComponentManager = std::make_unique<ComponentManager>();
    m_SystemManager = std::make_unique<SystemManager>();
    m_EntityManager = std::make_unique<EntityManager>();
  }

  static Coordinator& GetInstance() {
    static Coordinator* coordinator = new Coordinator();
    return *coordinator;	
  }

  // Entity methods
  Entity CreateEntity()
  {
    return m_EntityManager->CreateEntity();
  }

  void DestroyEntity(Entity entity)
  {
    //come on normalize these names smh
    m_EntityManager->DestroyEntity(entity);

    m_ComponentManager->entityDestroyed(entity);

    m_SystemManager->EntityDestroyed(entity);
  }


  // Component methods
  template<typename T>
    void RegisterComponent()
    {
      m_ComponentManager->registerComponent<T>();
    }

  template<typename T>
    void AddComponent(Entity entity, T component)
    {
      m_ComponentManager->addComponent<T>(entity, component);

      auto signature = m_EntityManager->getSignature(entity);
      signature.set(m_ComponentManager->getComponentType<T>(), true);
      m_EntityManager->setSignature(entity, signature);

      m_SystemManager->EntitySignatureChanged(entity, signature);
    }

  template<typename T>
    void RemoveComponent(Entity entity)
    {
      m_ComponentManager->removeComponent<T>(entity);

      auto signature = m_EntityManager->getSignature(entity);
      signature.set(m_ComponentManager->getComponentType<T>(), false);
      m_EntityManager->setSignature(entity, signature);

      m_SystemManager->EntitySignatureChanged(entity, signature);
    }

  template<typename T>
    T& GetComponent(Entity entity)
    {
      return m_ComponentManager->getComponent<T>(entity);
    }

  template<typename T>
  ComponentType GetComponentType()
    {
      return m_ComponentManager->getComponentType<T>();
    }


  // System methods
  template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
      return m_SystemManager->registerSystem<T>();
    }

  template<typename T>
  std::shared_ptr<T> GetSystem() {
    return m_SystemManager->GetSystem<T>();
  }
  
  template<typename T>
  void SetSystemSignature(Signature signature) {
      m_SystemManager->setSignature<T>(signature);
  }
protected:
  Coordinator() {};
 private:
  std::unique_ptr<ComponentManager> m_ComponentManager;
  std::unique_ptr<SystemManager> m_SystemManager;
  std::unique_ptr<EntityManager> m_EntityManager;
};
#endif 
