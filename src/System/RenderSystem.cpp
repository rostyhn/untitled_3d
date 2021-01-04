#include "RenderSystem.h"
#include "../Toolbox/Coordinator.h"
#include "../Components/Transform.h"
#include "../Components/Renderable.h"
#include "../Components/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

RenderSystem::RenderSystem() : m_shader("../res/shaders/simpleShader.vert","../res/shaders/simpleShader.frag"), m_perspective(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 10000.0f)) {
}

void RenderSystem::Init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  Loader* m_loader = &Loader::getInstance();
  
  OBJLoader::LoadObjModel("dragon", m_loader);
  m_loader->LoadTexture("dragon", false);
  m_loader->LoadTexture("scape", true);
  m_loader->LoadTexture("scape_alt", true);
  m_loader->LoadTexture("grassy2", true);
  m_loader->LoadTexture("cube", true);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Coordinator* coordinator = &Coordinator::GetInstance();  
  m_camera = coordinator->CreateEntity();
  coordinator->AddComponent(m_camera, Camera { glm::vec3(400.0f,2.0f,400.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) });
}

void RenderSystem::Render() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shader.use();
  Coordinator* coordinator = &Coordinator::GetInstance();
  Loader* m_loader = &Loader::getInstance();
 
  m_shader.setMat4("projection", m_perspective);
  
  auto const& camera = coordinator->GetComponent<Camera>(m_camera);
  m_shader.setMat4("view", camera.getView());
  
  for (auto const& e : m_Entities) {
    auto const& transform = coordinator->GetComponent<Transform>(e);
    auto const& renderable = coordinator->GetComponent<Renderable>(e);
    
    m_shader.setMat4("model", glm::translate(glm::mat4(1.0f), transform.position));
    glActiveTexture(GL_TEXTURE0);
    m_shader.setInt("ourTexture",0);

    glBindTexture(GL_TEXTURE_2D, m_loader->GetTextureID(renderable.textureName));
    glBindVertexArray(m_loader->GetVAO(renderable.modelName));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    
    
    glDrawElements(GL_TRIANGLES, m_loader->GetVertexCount(renderable.modelName), GL_UNSIGNED_INT, 0);
  }
}


