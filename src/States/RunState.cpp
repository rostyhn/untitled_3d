#include "RunState.h"

#include "../Components/Camera.h"
#include "../Components/Renderable.h"
#include "../Components/Transform.h"

#include "../States/MenuState.h"
#include "../System/ControlSystem.h"
#include "../System/RenderSystem.h"
#include "../System/ChunkSystem.h"

#include <math.h>

RunState::RunState() {
  // can be anywhere
  Coordinator *coordinator = &Coordinator::GetInstance();
  coordinator->Init();
  coordinator->RegisterComponent<Camera>();
  coordinator->RegisterComponent<Renderable>();
  coordinator->RegisterComponent<Transform>();
  auto renderSystem = coordinator->RegisterSystem<RenderSystem>();
  {
    Signature sig;
    sig.set(coordinator->GetComponentType<Renderable>());
    sig.set(coordinator->GetComponentType<Transform>());
    coordinator->SetSystemSignature<RenderSystem>(sig);
  }

  auto controlSystem = coordinator->RegisterSystem<ControlSystem>();
  {
    Signature sig;
    sig.set(coordinator->GetComponentType<Camera>());
    coordinator->SetSystemSignature<ControlSystem>(sig);
  }

  auto chunkSystem = coordinator->RegisterSystem<ChunkSystem>();
  {
    Signature sig;
    sig.set(coordinator->GetComponentType<Camera>());
    coordinator->SetSystemSignature<ChunkSystem>(sig);
  }

  renderSystem->Init();
  chunkSystem->Init();
  
  Entity e = coordinator->CreateEntity();
  coordinator->AddComponent(e, Renderable{"dragon", "dragon"});
  coordinator->AddComponent(e, Transform{glm::vec3(0.0f, -2.0f, -20.0f),
                                         glm::vec3(1.0f, 1.0f, 1.0f),
                                         glm::vec3(1.0f, 1.0f, 1.0f)});
  m_Entities.push_back(e);
}

void RunState::Init() {
  srand(time(0));
  this->t = 0.0;
  this->dt = 0.01;
  DisplayManager::getInstance().toggleCursor();
}

void RunState::Cleanup() {
  // could DRY this
  Coordinator *coordinator = &Coordinator::GetInstance();
  for (auto e : m_Entities) {
    coordinator->DestroyEntity(e);
  }
  m_Entities.clear();
}

void RunState::Pause() { this->paused = true; }

void RunState::Resume() { this->paused = false; }

void RunState::HandleEvents(GameManager *pManager) {
  // technically should be handled by a system
  Coordinator *coordinator = &Coordinator::GetInstance();
  if (pManager->m_keys[GLFW_KEY_ESCAPE] == GLFW_PRESS) {
    pManager->PushState(&MenuState::Instance());
  }

  auto controller = coordinator->GetSystem<ControlSystem>();
  controller->Update(pManager->m_keys,
                     glm::vec2(pManager->m_mouseX, pManager->m_mouseY));
}

void RunState::Update(GameManager *pManager) {
  Coordinator *coordinator = &Coordinator::GetInstance();
  auto chunkSystem = coordinator->GetSystem<ChunkSystem>();
  chunkSystem->Update();
}

void RunState::Draw(GameManager *pManager) {
  Coordinator *coordinator = &Coordinator::GetInstance();
  auto renderer = coordinator->GetSystem<RenderSystem>();
  renderer->Render();
}
