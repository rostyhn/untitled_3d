#include "GameManager.h"
#include "../States/GameState.h"
#include "../RenderEngine/DisplayManager.h"
#include <GL/glew.h>
#include <ctime>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h>

void GameManager::Init() {
  // Initialized GLFW
  if (glfwInit()) {

    // Create the display manager (pointers must be deleted)
    DisplayManager* m_displayManager = &DisplayManager::getInstance();

    // Initialize glew using experimental(new)
    glewExperimental = true;
    GLenum status = glewInit();
    if (status == GLEW_OK) {
      renderer = new MasterRenderer(m_displayManager->GetAspect());
      m_running = true;
    } else {
      std::cerr << "ERROR: GLEW failed to initialize \n"
                << glewGetErrorString(status) << std::endl;
    }
  } else {
    std::cerr << "ERROR: GLFW failed to initialize" << std::endl;
  }
}

void GameManager::Cleanup() {
  while (!states.empty()) {
    states.back()->Cleanup();
    states.pop_back();
  }
  delete &DisplayManager::getInstance();
  glfwTerminate();
}

void GameManager::ChangeState(GameState *state) {
  if (!states.empty()) {
    states.back()->Cleanup();
    states.pop_back();
  }

  states.push_back(state);
  states.back()->Init();
}

void GameManager::PushState(GameState *state) {
  // pause current state
  if (!states.empty()) {
    states.back()->Pause();
  }

  // store and init the new state
  states.push_back(state);
  states.back()->Init();
}

void GameManager::PopState() {
  // cleanup the current state
  if (!states.empty()) {
    states.back()->Cleanup();
    states.pop_back();
  }

  // resume previous state
  if (!states.empty()) {
    states.back()->Resume();
  }
}

void GameManager::HandleEvents() {
  // let the state handle events
  if(m_running) {
    states.back()->HandleEvents(this);
  }
}

void GameManager::Update() {
  // let the state update the game
  if(m_running) {
    states.back()->Update(this);
  }
}

void GameManager::Draw() {
  // let the state draw the screen
  if(m_running) {
    states.back()->Draw(this);
    DisplayManager::getInstance().UpdateDisplay();
    DisplayManager::getInstance().ShowUPS();
  }
}
