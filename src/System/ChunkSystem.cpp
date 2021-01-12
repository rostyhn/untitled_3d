#include "ChunkSystem.h"
#include "../RenderEngine/Loader.h"
#include "../Components/Camera.h"
#include "../Components/Renderable.h"
#include "../Components/Transform.h"
#include "../Components/Texture.h"
#include "../Components/Collidable.h"
#include "../Toolbox/Coordinator.h"
#include <vector>

void ChunkSystem::Init() {
  GenerateTerrain();
  
  LoadChunk("scape", glm::vec2(-1,-1));
  LoadChunk("scape", glm::vec2(-1, 0));
  LoadChunk("scape", glm::vec2(-1, 1));
  LoadChunk("scape", glm::vec2( 0,-1));
  LoadChunk("scape", glm::vec2( 0, 0));
  LoadChunk("scape", glm::vec2( 0, 1));
  LoadChunk("scape", glm::vec2( 1,-1));
  LoadChunk("scape", glm::vec2( 1, 0));
  LoadChunk("scape", glm::vec2( 1, 1));
}

void ChunkSystem::Update() {
  Coordinator* coordinator = &Coordinator::GetInstance();
  for (const auto& e : m_Entities) {  
    auto& cam = coordinator->GetComponent<Camera>(e);
    auto& transform = coordinator->GetComponent<Transform>(e);
    // - initial position of camera
    int x = (int) round((transform.position[0]-400) / (SIZE * CHUNKSIZE));
    int y = (int) round((transform.position[2]-400) / (SIZE * CHUNKSIZE));

    //need to generalize for bigger chunk sizes
    
    for(int i = -1; i < 2; i = i+2) {
      //check in the x directions
      if(m_Chunks.count(std::pair<int,int>(x+i,y)) == 0) {
	LoadChunk("scape", glm::vec2(x+i,y-1));
	LoadChunk("scape", glm::vec2(x+i,y));
	LoadChunk("scape", glm::vec2(x+i,y+1));

	int negI = -(CHUNKSATATIME-1) * i;
	UnloadChunk(glm::vec2(x+negI,y-1));
	UnloadChunk(glm::vec2(x+negI,y));
	UnloadChunk(glm::vec2(x+negI,y+1));
      }
      //check in the y directions
      if(m_Chunks.count(std::pair<int,int>(x,y+i)) == 0) {
	LoadChunk("scape", glm::vec2(x-1,y+i));
	LoadChunk("scape", glm::vec2(x,y+i));
	LoadChunk("scape", glm::vec2(x+1,y+i));

	int negI = -(CHUNKSATATIME-1) * i;
	UnloadChunk(glm::vec2(x-1,y+negI));
	UnloadChunk(glm::vec2(x,y+negI));
	UnloadChunk(glm::vec2(x+1,y+negI));
	}
      //check diagonals
      if(m_Chunks.count(std::pair<int,int>(x+i,y+i)) == 0) {
	LoadChunk("scape", glm::vec2(x+i,y+i));

	//int negI = -(CHUNKSIZE-1) * i;
	//UnloadChunk(glm::vec2(x+negI,y+negI));
      }
     }
    
  }
}

void ChunkSystem::UnloadChunk(glm::vec2 position) {
  Coordinator* coordinator = &Coordinator::GetInstance();
  std::pair<int,int> coords = std::pair<int,int>(position[0],position[1]);
  std::vector<Entity> eList = m_Chunks[coords];
  for(auto& e : eList) {
    coordinator->DestroyEntity(e);
  }
  eList.clear();
  m_Chunks.erase(coords);
}

void ChunkSystem::LoadChunk(const char* textureName, glm::vec2 position) {
  
  float x = CHUNKSIZE * (float) position[0] * SIZE;
  float y = CHUNKSIZE * (float) position[1] * SIZE;
  std::vector<Entity> eList;

  int half = (int)CHUNKSIZE/2;
  
  for(int i = -half; i <= half; i++) {
    for(int j = -half; j <= half; j++) {
      eList.push_back(LoadTerrain("terrain1", textureName, glm::vec2(x+(i*SIZE),y+(j*SIZE))));
    }
  }    
  //holy shit this sucks lol
  m_Chunks.insert(std::pair<std::pair<int,int>,std::vector<Entity>>(std::pair<int,int>(position[0],position[1]), eList));
}

Entity ChunkSystem::LoadTerrain(const char* modelName, const char* textureName, glm::vec2 position) {
  Coordinator* coordinator = &Coordinator::GetInstance();
  Entity e = coordinator->CreateEntity();  
  coordinator->AddComponent(e, Transform { glm::vec3(position[0],0.0f,position[1]), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)});
  coordinator->AddComponent(e, Renderable { modelName });
  coordinator->AddComponent(e, Texture { textureName, 20.0f });
  //coordinator->AddComponent(e, Collidable { 0.0f, -1.0f, 0.0f, 800.0f, 0.25, 800.0f, true });
  return e;
}

void ChunkSystem::Cleanup() {
  m_Chunks.clear();
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
  std::vector<float> bBox = { 0.0f, -1.0f, 0.0f, float(count), 1.0f, float(count)  };
  return loader->LoadToVAO(vertices, textureCoords, normals, indices,
                           "terrain1", bBox);
}
