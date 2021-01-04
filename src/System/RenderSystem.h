#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"
#include "../Shaders/Shader.h"
#include "../RenderEngine/Loader.h"
#include "../RenderEngine/OBJLoader.h"

#include <memory>

class RenderSystem : public System {
 public:
  RenderSystem();
  void Init();  
  void Render();
  void GenerateTerrainModel(const std::string& name);
 private:
  Shader m_shader;
  Entity m_camera;
  glm::mat4 m_perspective;
  
};
#endif
