#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "TypeDefinitions.h"
#include <any>
#include <cassert>
#include <unordered_map>

class IComponentArray
{
 public:
  virtual ~IComponentArray() = default;
  virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T>
class ComponentArray : public IComponentArray {
 public:
  void InsertData(Entity entity, T component){
    // Put new entry at end
    size_t newIndex = mSize;
    mEntityToIndexMap[entity] = newIndex;
    mIndexToEntityMap[newIndex] = entity;
    mComponentArray[newIndex] = component;
    ++mSize;
  }

  void RemoveData(Entity entity) {
    // Copy element at end into deleted element's place to maintain density
    size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
    size_t indexOfLastElement = mSize - 1;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    // Update map to point to moved spot
    Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entity);
    mIndexToEntityMap.erase(indexOfLastElement);

    --mSize;
  }

  T& GetData(Entity entity) {
      return mComponentArray[mEntityToIndexMap[entity]];
    }

  void EntityDestroyed(Entity entity) override {
    if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
      {
	RemoveData(entity);
      }
  }

 private:
  std::array<T, MAX_ENTITIES> mComponentArray{};
  std::unordered_map<Entity, size_t> mEntityToIndexMap{};
  std::unordered_map<size_t, Entity> mIndexToEntityMap{};
  size_t mSize{};
};
#endif
