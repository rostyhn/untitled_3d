#ifndef CHUNKSYSTEM_H
#define CHUNKSYSTEM_H

#include "System.h"
#include <glm/glm.hpp>
#include <vector>

class ChunkSystem : public System {
public:
  ChunkSystem() {};
  void Init();
  void Update();
  
private:
  void GenerateTerrain();
  void LoadChunk(const char* textureName, glm::vec2 position);
  void LoadTerrain(const char* modelName, const char* textureName, glm::vec2 position);
  //if we decide to generate random terrain
  //int m_counter = 0;
  const float SIZE = 800;
  const int VERTEX_COUNT = 64;
  std::vector<Entity> m_terrains;
};
#endif
