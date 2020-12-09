#include "MenuState.h"
#include "../RenderEngine/DisplayManager.h"
/* TODO:
 * figure out how to get the current height / width of the window and position stuff relatively to the screen
 * have this passed to the menuItems to facilitate collision detection
 * have a piece of the runstate saved as a runbuffer and blurred to make it look pretty that would be cool
 * figure out if submenus should be another state or maybe even make a menuManager class
 * figure out how to exit gracefully
 */

enum MenuCommand {
	CONT,
	QUIT
};


void MenuState::Init()
{
	menuItems.push_back(MenuItem{ "Continue", 500.0f, 750.0f, 1.0f, glm::vec3(1.0f,1.0f,1.0f), CONT });
	menuItems.push_back(MenuItem{ "Quit", 500.0f, 500.0f, 1.0f, glm::vec3(1.0f,1.0f,1.0f), QUIT });
}

void MenuState::Cleanup()
{
	menuItems.clear();
}

void MenuState::Pause()
{
}

void MenuState::Resume()
{
}

void MenuState::HandleEvents(GameManager * pManager)
{
        DisplayManager* displayManager = &DisplayManager::getInstance();
	double xpos, ypos;
	glfwGetCursorPos(displayManager->getWindow(), &xpos, &ypos);

	

	//check for collision between cursor and menu elements
	for (MenuItem& item : menuItems)
	{
		bool collided = item.checkCollision(xpos, ypos);
		if (collided)
		{
			if (glfwGetMouseButton(displayManager->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				MenuCommand menuCommand = (MenuCommand) item.menuCommand;

				switch (menuCommand)
				{
					case QUIT:
						pManager->Cleanup();
						pManager->Quit();
						break;
					case CONT:
					default:
					        DisplayManager::getInstance().toggleCursor();
						pManager->PopState();
						break;
				}
			}
		}
	}
}

void MenuState::Update(GameManager * pManager)
{

}

void MenuState::Draw(GameManager * pManager)
{
	MasterRenderer* renderer = pManager->renderer;
	
	for (MenuItem& mi : menuItems)
	{
		renderer->Process2DText(mi);
	}
	renderer->RenderMenu();
}
