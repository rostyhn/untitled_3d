#include "RunState.h"

#include "../Components/Camera.h"
#include "../Components/Renderable.h"
#include "../Components/Transform.h"
#include "../Components/Texture.h"
#include "../Components/Collidable.h"
#include "../Components/AABB.h"

#include "../States/MenuState.h"

#include "../System/ControlSystem.h"
#include "../System/RenderSystem.h"
#include "../System/ChunkSystem.h"
#include "../System/CollisionSystem.h"

#include <math.h>
#include <glm/glm.hpp>

RunState::RunState() {
  // can be anywhere
  Coordinator *coordinator = &Coordinator::GetInstance();
  coordinator->Init();
  coordinator->RegisterComponent<Camera>();
  coordinator->RegisterComponent<Renderable>();
  coordinator->RegisterComponent<Transform>();
  coordinator->RegisterComponent<Texture>();
  coordinator->RegisterComponent<Collidable>();
  coordinator->RegisterComponent<AABB>();
  
  auto renderSystem = coordinator->RegisterSystem<RenderSystem>();
  {
    Signature sig;
    sig.set(coordinator->GetComponentType<Renderable>());
    sig.set(coordinator->GetComponentType<Transform>());
    sig.set(coordinator->GetComponentType<Texture>());
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

  auto collisionSystem = coordinator->RegisterSystem<CollisionSystem>();
  {
    Signature sig;
    sig.set(coordinator->GetComponentType<Collidable>());
    sig.set(coordinator->GetComponentType<Transform>());
    coordinator->SetSystemSignature<CollisionSystem>(sig);
  }
  
  renderSystem->Init();
  chunkSystem->Init();

  srand(time(0));
  
  for(int i = 0; i < 1000; i++) {
    Entity e = coordinator->CreateEntity();
    coordinator->AddComponent(e, Renderable{"box"});
    coordinator->AddComponent(e, Texture {"box", 20.0f});

    coordinator->AddComponent(e, Transform{glm::vec3(rand() % 10000, rand() % 10000, rand() % 10000),
					   glm::vec3(0.0f, 0.0f, 0.0f),
                                         glm::vec3(2.0f, 2.0f, 2.0f)
                                         });
    coordinator->AddComponent(e, Collidable {"box", false});
  
    m_Entities.push_back(e);
  }
}

void RunState::Init() {  
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
  auto collisionSystem = coordinator->GetSystem<CollisionSystem>();
  collisionSystem->Update();
  chunkSystem->Update();
}

void RunState::Draw(GameManager *pManager) {
  Coordinator *coordinator = &Coordinator::GetInstance();
  auto renderer = coordinator->GetSystem<RenderSystem>();
  renderer->Render();
  t += dt;
}
