#include "UICollisionSystem.h"
#include "../Components/Transform.h"
#include "../Components/UIElement.h"
#include "../RenderEngine/DisplayManager.h"
#include "../Toolbox/Coordinator.h"

#include <stdlib.h>

void UICollisionSystem::Update(const glm::vec2 mousePos,
                               const bool *mouseButtons,
                               GameManager *pManager) {

  Coordinator *coordinator = &Coordinator::GetInstance();

  for (const auto &e : m_Entities) {
    auto &ui = coordinator->GetComponent<UIElement>(e);
    // opengl mouse coordinates start from the top left instead of bottom left
    int w, h;
    glfwGetFramebufferSize(DisplayManager::getInstance().getWindow(), &w, &h);

    float xPos = mousePos[0];
    float yPos = abs(mousePos[1] - h);

    if ((xPos > ui.xMin && xPos < ui.xMax) &&
        (yPos > ui.yMin && yPos < ui.yMax)) {
      ui.color = glm::vec3(1.0f, 0.0f, 0.0f);
      if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
        ui.color = glm::vec3(1.0, 1.0, 0.0);
        switch (ui.command) {
        case QUIT:
          pManager->Cleanup();
          pManager->Quit();
          break;
        case CONT:
        default:
          DisplayManager *disp = &DisplayManager::getInstance();
          disp->toggleCursor();
          pManager->PopState();
          break;
        }
      }
    }
  else {
    ui.color = glm::vec3(1.0f, 1.0f, 1.0f);
  }
}
}
