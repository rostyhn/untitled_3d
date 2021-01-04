#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "System.h"
#include <memory>
#include <unordered_map>

class SystemManager {
public:
  template <typename T> std::shared_ptr<T> registerSystem() {
    const char *typeName = typeid(T).name();
    auto system = std::make_shared<T>();
    m_Systems.insert({typeName, system});
    return system;
  }

  template <typename T> void setSignature(Signature sig) {
    const char *typeName = typeid(T).name();
    m_Signatures.insert({typeName, sig});
  }

  void EntityDestroyed(Entity entity) {
    for (auto const &pair : m_Systems) {
      auto const &system = pair.second;
      system->m_Entities.erase(entity);
    }
  }

  void EntitySignatureChanged(Entity e, Signature eSig) {

    for (auto const &pair : m_Systems) {
      auto const &type = pair.first;
      auto const &sys = pair.second;
      auto const &sysSig = m_Signatures[type];

      if ((eSig & sysSig) == sysSig) {
        sys->m_Entities.insert(e);
      } else {
        sys->m_Entities.erase(e);
      }
    }
  }

  template <typename T>
  std::shared_ptr<T> GetSystem() {
      const char* typeName = typeid(T).name();    
      return std::static_pointer_cast<T>(m_Systems[typeName]);
  }
  
private:
  std::unordered_map<const char *, Signature> m_Signatures{};
  std::unordered_map<const char *, std::shared_ptr<System>> m_Systems;
  
};

#endif
