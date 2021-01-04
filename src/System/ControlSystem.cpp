#include "ControlSystem.h"
#include "../Components/Camera.h"
#include "../Toolbox/Coordinator.h"

#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

void ControlSystem::Update(const bool* keys, const glm::vec2 mousePos) {
  Coordinator* coordinator = &Coordinator::GetInstance();
  
  for(auto const& e : m_Entities) {
    auto& camera = coordinator->GetComponent<Camera>(e);
    const float cameraSpeed = 1.5f;
    glm::vec2 mouseDel = mousePos - camera.mousePosition;

    if(glm::length(mouseDel) > 100) {
      camera.mousePosition = mousePos;
      return;
    }

    glm::vec3 vAxis = glm::cross(camera.front, camera.up);

    camera.front = glm::normalize(glm::mat3(
		glm::rotate(-mouseDel.x * camera.sensitivity, camera.up) *
		glm::rotate(-mouseDel.y * camera.sensitivity, vAxis)
		) * camera.front);

	// Update the mouse position
    camera.mousePosition = mousePos;
    float multiplier = 1.0f;

    if(keys[GLFW_KEY_LEFT_SHIFT]) {
      multiplier = 2.0f;
    }
    
    if(keys[GLFW_KEY_W]) {
      camera.position += multiplier * cameraSpeed * camera.front;
    }
    if(keys[GLFW_KEY_S]) {
      camera.position -= cameraSpeed * camera.front;
    }
    if(keys[GLFW_KEY_A]) {
      camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    }
    if(keys[GLFW_KEY_D]) {
      camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    }
    
  }
}
