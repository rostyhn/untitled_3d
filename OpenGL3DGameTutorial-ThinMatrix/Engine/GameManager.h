#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <GL\glew.h>
#include "../RenderEngine/DisplayManager.h"
#include "../RenderEngine/Loader.h"
#include "../RenderEngine/MasterRenderer.h"
#include "../Models/RawModel.h"
#include "../Models/TexturedModel.h"
#include "../Textures/ModelTexture.h"
#include "../Shaders/BasicShader.h"
#include "../Entities/Entity.h"
#include "../Entities/Camera.h"
#include "../Entities/FPSCamera.h"
#include "../RenderEngine/OBJLoader.h"
#include "../Terrain/Terrain.h"


class GameManager
{
public:
	GameManager();
	virtual ~GameManager();
	void Start();
private:
	void generateNewLand(glm::vec3 position, std::vector<Terrain>& terrains, std::vector<Entity>& entities, Loader &loader);
	void generateEntities(glm::vec3 position, int minX, int minZ, int maxX, int maxZ);
	void simulate(float dt);

	std::vector<Entity> entities;
	std::vector<Terrain> terrains;
	std::vector<TexturedModel> texModels;

	DisplayManager* m_displayManager;
};

#endif // GAMEMANAGER_H