#include "ControlSystem.h"
#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../Components/Physics.h"
#include "../Toolbox/Coordinator.h"

#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

void ControlSystem::Update(const bool* keys, const glm::vec2 mousePos, const float dt) {
  Coordinator* coordinator = &Coordinator::GetInstance();
  
  for(auto const& e : m_Entities) {
    auto& camera = coordinator->GetComponent<Camera>(e);
    auto& transform = coordinator->GetComponent<Transform>(e);
    auto& phy = coordinator->GetComponent<Physics>(e);
    
    const float cameraSpeed = 1.0f;
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

    if(camera.fly) {
    
      if(keys[GLFW_KEY_LEFT_SHIFT]) {
	multiplier = 5.0f;
      }
    
      if(keys[GLFW_KEY_W]) {
	transform.position += multiplier * cameraSpeed * camera.front;
      }
      if(keys[GLFW_KEY_S]) {
	transform.position -= cameraSpeed * camera.front;
      }
      if(keys[GLFW_KEY_A]) {
	transform.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
      }
      if(keys[GLFW_KEY_D]) {
	transform.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
      }
    } else {
      if(keys[GLFW_KEY_LEFT_SHIFT]) {
	multiplier = 2.5f;
      }
      
      if(keys[GLFW_KEY_W]) {
	transform.position += multiplier * cameraSpeed * glm::vec3(camera.front[0], 0.0f, camera.front[2]);
      }
      if(keys[GLFW_KEY_S]) {
	transform.position += (cameraSpeed * -glm::vec3(camera.front[0], 0.0f, camera.front[2]));
      }
      if(keys[GLFW_KEY_A]) {
	transform.position += (-glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed);
      }
      if(keys[GLFW_KEY_D]) {
	transform.position += (glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed);
      }
      
      if(keys[GLFW_KEY_SPACE]) {
	phy.AddForce(glm::vec3(0.0f,25.0f,0.0f));
      }
    }
    //later, gonna add console commands, that would be really cool
      if(keys[GLFW_KEY_V]) {
	std::cout << "You are at" << std::endl;
	std::cout << "X: " << transform.position[0] << std::endl;
	std::cout << "Y: " << transform.position[1] << std::endl;
	std::cout << "Z: " << transform.position[2] << std::endl;
      }

    
  }
}
