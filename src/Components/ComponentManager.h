#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "../Toolbox/ComponentArray.h"
#include "../Toolbox/TypeDefinitions.h"

#include <unordered_map>
#include <memory>

class ComponentManager {
 public:
    template <typename T>
    void registerComponent() {
      const char* typeName = typeid(T).name();
      
      m_componentTypes.insert({typeName, m_nextComponentType});
      m_componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

      m_nextComponentType++;
    }

    template <typename T>
    ComponentType getComponentType() {     
      const char* typeName = typeid(T).name();
      return m_componentTypes[typeName];
    }

    template <typename T>
    void addComponent(Entity e, T comp) {
      GetComponentArray<T>()->InsertData(e, comp);
    }

    template <typename T>
    void removeComponent(Entity e) {
      GetComponentArray<T>()->RemoveData(e);
    }

    template <typename T>
    T& getComponent(Entity e) {
      return GetComponentArray<T>()->GetData(e);
    }

    void entityDestroyed(Entity e) {
      for (auto const& pair : m_componentArrays) {
	auto const& component = pair.second;
	component->EntityDestroyed(e);
      }
    }
  
 private:
    std::unordered_map<const char*, ComponentType> m_componentTypes{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays{};
  
    ComponentType m_nextComponentType;
  
    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
      const char* typeName = typeid(T).name();
      return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
    }
};
#endif
