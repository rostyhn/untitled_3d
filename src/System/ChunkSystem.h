#ifndef CHUNKSYSTEM_H
#define CHUNKSYSTEM_H

#include "System.h"
#include <glm/glm.hpp>
//to use unordered_map: https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
#include <map>
#include <vector>


class ChunkSystem : public System {
public:
  ChunkSystem() {};
  void Init();
  void Update();
  void Cleanup();
private:
  void GenerateTerrain();
  void UnloadChunk(glm::vec2 position);
  void LoadChunk(const char* textureName, glm::vec2 position);
  Entity LoadTerrain(const char* modelName, const char* textureName, glm::vec2 position);
  //if we decide to generate random terrain
  //int m_counter = 0;
  const float SIZE = 800;
  const float CHUNKSIZE = 9;
  const int CHUNKSATATIME = 3;
  const int VERTEX_COUNT = 64;
  std::map<std::pair<int, int>, std::vector<Entity>> m_Chunks;
};
#endif
