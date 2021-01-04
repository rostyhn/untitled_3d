#include "MenuState.h"
#include "../Components/Transform.h"
#include "../Components/UIElement.h"
#include "../RenderEngine/DisplayManager.h"
#include "../System/UICollisionSystem.h"
#include "../System/UIRenderSystem.h"
/* TODO:
 * figure out how to get the current height / width of the window and position
 * stuff relatively to the screen have this passed to the menuItems to
 * facilitate collision detection have a piece of the runstate saved as a
 * runbuffer and blurred to make it look pretty that would be cool figure out if
 * submenus should be another state or maybe even make a menuManager class
 * figure out how to exit gracefully
 */

MenuState::MenuState() {
  
  Coordinator *coordinator = &Coordinator::GetInstance();
  coordinator->RegisterComponent<UIElement>();

  auto uiRenderSystem = coordinator->RegisterSystem<UIRenderSystem>();
  {
    Signature sig;
    sig.set(coordinator->GetComponentType<Transform>());
    sig.set(coordinator->GetComponentType<UIElement>());
    coordinator->SetSystemSignature<UIRenderSystem>(sig);
  }

  uiRenderSystem->Init();

  auto uiCollisionSystem = coordinator->RegisterSystem<UICollisionSystem>();
  {
    Signature sig;
    sig.set(coordinator->GetComponentType<UIElement>());
    coordinator->SetSystemSignature<UICollisionSystem>(sig);
  }
}

void MenuState::Init() {
  DisplayManager *displayManager = &DisplayManager::getInstance();
  displayManager->toggleCursor();
  Coordinator *coordinator = &Coordinator::GetInstance();

  Entity cont = coordinator->CreateEntity();
  coordinator->AddComponent(
      cont, UIElement{"Continue", 1.0f, glm::vec3(1.0, 1.0, 1.0), CONT});
  coordinator->AddComponent(cont, Transform{glm::vec3(250.0f, 600.0f, 0.0f),
                                            glm::vec3(1.0f, 1.0f, 1.0f),
                                            glm::vec3(1.0f, 1.0f, 1.0f)});

  Entity quit = coordinator->CreateEntity();
  coordinator->AddComponent(
      quit, UIElement{"Quit", 1.0f, glm::vec3(1.0, 1.0, 1.0), QUIT});
  coordinator->AddComponent(quit, Transform{glm::vec3(250.0f, 450.0f, 0.0f),
                                            glm::vec3(1.0f, 1.0f, 1.0f),
                                            glm::vec3(1.0f, 1.0f, 1.0f)});
  m_Entities.push_back(cont);
  m_Entities.push_back(quit);
}

void MenuState::Cleanup() {
  Coordinator *coordinator = &Coordinator::GetInstance();
  for (auto e : m_Entities) {
    coordinator->DestroyEntity(e);
  }
  m_Entities.clear();
}

void MenuState::Pause() {}

void MenuState::Resume() {}

void MenuState::HandleEvents(GameManager *pManager) {
  Coordinator *coordinator = &Coordinator::GetInstance();
  auto collider = coordinator->GetSystem<UICollisionSystem>();
  collider->Update(glm::vec2(pManager->m_mouseX, pManager->m_mouseY),
                   pManager->m_mouse_buttons, pManager);
}

void MenuState::Update(GameManager *pManager) {}

void MenuState::Draw(GameManager *pManager) {
  Coordinator *coordinator = &Coordinator::GetInstance();
  auto uirenderer = coordinator->GetSystem<UIRenderSystem>();
  uirenderer->Render();
}
