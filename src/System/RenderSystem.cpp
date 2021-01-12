#include "RenderSystem.h"
#include "../Toolbox/Coordinator.h"
#include "../Components/Transform.h"
#include "../Components/Texture.h"
#include "../Components/Renderable.h"
#include "../Components/Camera.h"
#include "../Components/Collidable.h"
#include "../Components/AABB.h"
#include "../Components/Physics.h"
#include "../Toolbox/Maths.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

RenderSystem::RenderSystem() : m_shader("../res/shaders/simpleShader.vert","../res/shaders/simpleShader.frag"), m_perspective(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 10000.0f)) {
			      
}

void RenderSystem::Init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  Loader* m_loader = &Loader::getInstance();
  
  OBJLoader::LoadObjModel("box", m_loader);
  OBJLoader::LoadObjModel("dragon", m_loader);
  
  m_loader->LoadTexture("box", false);
  m_loader->LoadTexture("scape", true);
  m_loader->LoadTexture("scape_alt", true);
  m_loader->LoadTexture("grassy2", true);
  m_loader->LoadTexture("cube", true);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Coordinator* coordinator = &Coordinator::GetInstance();  
  m_camera = coordinator->CreateEntity();
  coordinator->AddComponent(m_camera, Camera { false, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) });
  coordinator->AddComponent(m_camera, Transform { glm::vec3(400.0f, 4.0f, 400.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f) });
  coordinator->AddComponent(m_camera, Collidable { -2.0, -2.0f, -2.0f, 2.0f, 2.0f, 2.0f});
  coordinator->AddComponent(m_camera, Physics { glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), 1.0f});
}

void RenderSystem::Render() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shader.use();
  Coordinator* coordinator = &Coordinator::GetInstance();
  Loader* m_loader = &Loader::getInstance();
 
  m_shader.setMat4("projection", m_perspective);
  
  auto const& camera = coordinator->GetComponent<Camera>(m_camera);
  auto const& camera_transform = coordinator->GetComponent<Transform>(m_camera);
  
  m_shader.setMat4("view", glm::lookAt(camera_transform.position, camera_transform.position + camera.front, camera.up));
  
  for (auto const& e : m_Entities) {
    auto const& transform = coordinator->GetComponent<Transform>(e);
    auto const& renderable = coordinator->GetComponent<Renderable>(e);
    auto const& texture = coordinator->GetComponent<Texture>(e);
    glm::mat4 model = Maths::CreateTransformMatrix(transform.position, transform.rotation, transform.scale);
    
    m_shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    m_shader.setInt("ourTexture",0);
    m_shader.setFloat("tileAmount", texture.tileAmount);
    glBindTexture(GL_TEXTURE_2D, m_loader->GetTextureID(texture.textureName));

    glBindVertexArray(m_loader->GetVAO(renderable.modelName));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glDrawElements(GL_TRIANGLES, m_loader->GetVertexCount(renderable.modelName), GL_UNSIGNED_INT, 0);    
  }
}


