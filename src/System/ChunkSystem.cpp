#include "ChunkSystem.h"
#include "../RenderEngine/Loader.h"
#include "../Components/Camera.h"
#include "../Components/Renderable.h"
#include "../Components/Transform.h"
#include "../Toolbox/Coordinator.h"
#include <vector>

void ChunkSystem::Init() {
  GenerateTerrain();
  LoadChunk("scape", glm::vec2(-1.0f,-1.0f));
  LoadChunk("scape", glm::vec2(-1.0f,0.0f));
  LoadChunk("scape", glm::vec2(-1.0f,1.0f));
  LoadChunk("scape", glm::vec2(0.0f,-1.0f));
  LoadChunk("scape", glm::vec2(0.0f,0.0f));
  LoadChunk("scape", glm::vec2(0.0f,1.0f));
  LoadChunk("scape", glm::vec2(1.0f,-1.0f));
  LoadChunk("scape", glm::vec2(1.0f,0.0f));
  LoadChunk("scape", glm::vec2(1.0f,1.0f));
}

void ChunkSystem::Update() {
  Coordinator* coordinator = &Coordinator::GetInstance();
  for (const auto& e : m_Entities) {  
    auto& cam = coordinator->GetComponent<Camera>(e);
  }
}

void ChunkSystem::LoadChunk(const char* textureName, glm::vec2 position) {
  //lets say 0,0 ; x = x * SIZE; y = y * SIZE
  const float chunkSize = 3;
  float x = chunkSize * position[0] * SIZE;
  float y = chunkSize * position[1] * SIZE;
  LoadTerrain("terrain1", textureName, glm::vec2(x,        y + SIZE)); 
  LoadTerrain("terrain1", textureName, glm::vec2(x + SIZE, y + SIZE)); 
  LoadTerrain("terrain1", textureName, glm::vec2(x - SIZE, y + SIZE)); 
  LoadTerrain("terrain1", textureName, glm::vec2(x,y)); 
  LoadTerrain("terrain1", textureName, glm::vec2(x - SIZE, y)); 
  LoadTerrain("terrain1", textureName, glm::vec2(x + SIZE,y)); 
  LoadTerrain("terrain1", textureName, glm::vec2(x ,y - SIZE));
  LoadTerrain("terrain1", textureName, glm::vec2(x - SIZE, y - SIZE)); 
  LoadTerrain("terrain1", textureName, glm::vec2(x + SIZE, y - SIZE));
  
}

void ChunkSystem::LoadTerrain(const char* modelName, const char* textureName, glm::vec2 position) {

  Coordinator* coordinator = &Coordinator::GetInstance();
  Entity e = coordinator->CreateEntity();  
  coordinator->AddComponent(e, Transform { glm::vec3(position[0],0.0f,position[1]), glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.0f,0.0f,0.0f) });
  coordinator->AddComponent(e, Renderable { modelName, textureName });
  m_terrains.push_back(e);
}



void ChunkSystem::GenerateTerrain() {
  int count = VERTEX_COUNT * VERTEX_COUNT;
  std::vector<glm::vec3> vertices(count), normals(count);
  std::vector<glm::vec2> textureCoords(count);
  std::vector<int> indices(6 * (VERTEX_COUNT - 1) * VERTEX_COUNT);

  int vertexPointer = 0;
  for (int i = 0; i < VERTEX_COUNT; i++) {
    for (int j = 0; j < VERTEX_COUNT; j++) {
      vertices[vertexPointer] =
          glm::vec3((float)j / ((float)VERTEX_COUNT - 1) * SIZE, 0,
                    (float)i / ((float)VERTEX_COUNT - 1) * SIZE);
      normals[vertexPointer] = glm::vec3(0, 1, 0);
      textureCoords[vertexPointer] =
          glm::vec2((float)j / ((float)VERTEX_COUNT - 1),
                    (float)i / ((float)VERTEX_COUNT - 1));
      vertexPointer++;
    }
  }
  int pointer = 0;
  for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
    for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
      int topLeft = (gz * VERTEX_COUNT) + gx;
      int topRight = topLeft + 1;
      int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
      int bottomRight = bottomLeft + 1;
      indices[pointer++] = topLeft;
      indices[pointer++] = bottomLeft;
      indices[pointer++] = topRight;
      indices[pointer++] = topRight;
      indices[pointer++] = bottomLeft;
      indices[pointer++] = bottomRight;
    }
  }

  Loader* loader = &Loader::getInstance();

  return loader->LoadToVAO(vertices, textureCoords, normals, indices,
                           "terrain1");
}
