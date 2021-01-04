#include "Engine/GameManager.h"
#include "../src/States/RunState.h"
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
GameManager gameManager;

int main(int argc, char** argv)
{

	gameManager.Init();
	glfwSetKeyCallback(DisplayManager::getInstance().getWindow(), key_callback); 
	glfwSetCursorPosCallback(DisplayManager::getInstance().getWindow(), mouse_callback);
	glfwSetMouseButtonCallback(DisplayManager::getInstance().getWindow(), mouse_button_callback);
	
	gameManager.ChangeState(&RunState::Instance());

	while (gameManager.Running()) {
		gameManager.HandleEvents();
		gameManager.Update();
		gameManager.Draw();
	}
	
	gameManager.Cleanup();
	return 0;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  //trickle down to gameManager, have it do whatever it wants with this info
  if(key >= 0 && key < 1024) {
    if(action == GLFW_PRESS) {
      gameManager.m_keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
      gameManager.m_keys[key] = false;
    }
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  gameManager.m_mouseX = xpos;
  gameManager.m_mouseY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if(button >= 0 && button < 8) {
    if(action == GLFW_PRESS) {
      gameManager.m_mouse_buttons[button] = true;
    }
    else if (action == GLFW_RELEASE) {
      gameManager.m_mouse_buttons[button] = false;
    }
  }
}
