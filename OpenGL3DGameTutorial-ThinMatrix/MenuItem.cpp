#include "MenuItem.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/*TODO:
 * figure out how to get the bounding box of text you avoid shooting in the dark when doing collision detection
 * make this into a baseclass and extend it further to include buttons, switches etc
 * could use this for the HUD too eventually
 * as mentioned before, either push down the width height for collision detection or just push the correct cursor pos the first time
 */

MenuItem::MenuItem(std::string text, float x, float y, float scale, glm::vec3 color, int menuCommand)
{
	this->menuCommand = menuCommand;
	this->text = text;
	this->x = x;
	this->y = y;
	this->scale = scale;
	this->color = color;
}

bool MenuItem::checkCollision(double xPos, double yPos)
{
	bool collided = false;
	//dirty conversion for yPos because we're relative to top left but drawing relative to bottom left
	yPos = abs(yPos - 1080);
	std::cout << "X: " << xPos << " Y: " << yPos << std::endl;

	//would be nice to calculate actual bounding box but whatever for now

	if (xPos < this->x + 100 && xPos > this->x - 100 && yPos < this->y + 100 && yPos > this->y - 50)
	{
		this->color = glm::vec3(0.0, 1.0, 1.0);
		this->scale = 2.0;
		collided = true;
	}
	else
	{
		this->color = glm::vec3(1.0, 1.0, 1.0);
		this->scale = 1.0;
	}
	return collided;
}

MenuItem::~MenuItem()
{
}
