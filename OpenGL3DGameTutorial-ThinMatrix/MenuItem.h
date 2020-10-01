#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#include <string>
#include <glm/glm.hpp>

class MenuItem
{
public:
	MenuItem(std::string text, float x, float y, float scale, glm::vec3 color, int menuCommand);
	std::string text;
	int menuCommand;
	float x;
	float y;
	float scale;
	glm::vec3 color;
	bool checkCollision(double xPos, double yPos);


	~MenuItem();
};

#endif // !MENU_ITEM_H
