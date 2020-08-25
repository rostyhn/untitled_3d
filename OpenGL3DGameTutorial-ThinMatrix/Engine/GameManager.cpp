#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <ctime>
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include "GameManager.h"
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
#include <random>
#include <chrono>
#include <thread>



GameManager::GameManager()
{
	// Initialized GLFW
	if (glfwInit())
	{
		std::cout << "GLFW initialized successfully" << std::endl;

		// Create the display manager (pointers must be deleted)
		m_displayManager = new DisplayManager(1920, 1080, "Untitled");

		// Initialize glew using experimental(new)
		glewExperimental = true;
		GLenum status = glewInit();
		if (status == GLEW_OK)
		{
			std::cout << "GLEW initialized successfully" << std::endl;
		}
		else
		{
			std::cerr << "ERROR: GLEW failed to initialize \n" << glewGetErrorString(status) << std::endl;
		}
	}
	else
	{
		std::cerr << "ERROR: GLFW failed to initialize" << std::endl;
	}
}


GameManager::~GameManager()
{
	// Delete the display and clean up GLFW
	delete m_displayManager;
	glfwTerminate();
}

void GameManager::Start()
{
	std::cout << "Game loop is now running" << std::endl;
	// START temporary data
	Loader loader;


	srand(time(0));
	float t = 0.0;
	float dt = 0.01;

	

	/*RawModel mBox = OBJLoader::LoadObjModel("lowPolyTree", loader);
	ModelTexture mtBox(loader.LoadTexture("lowPolyTree"));
	TexturedModel tmBox(mBox, mtBox);
	for (int i = 0; i < 25; i++){
		int x = rand() % 500 - 250;
		int z = rand() % 500 - 250;
		entities.push_back(Entity(tmBox, glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	}
	
	RawModel mGrass = OBJLoader::LoadObjModel("grass", loader);
	ModelTexture mtGrass(loader.LoadTexture("grass"), true, true);
	TexturedModel tmGrass(mGrass, mtGrass);
	for (int i = 0; i < 100; i++){
		int x = rand() % 500 - 250;
		int z = rand() % 500 - 250;
		entities.push_back(Entity(tmGrass, glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	}

	RawModel mFern = OBJLoader::LoadObjModel("fern", loader);
	ModelTexture mtFern1(loader.LoadTexture("fern1"), true, true);
	ModelTexture mtFern2(loader.LoadTexture("fern2"), true, true);
	ModelTexture mtFern3(loader.LoadTexture("fern3"), true, true);
	ModelTexture mtFern4(loader.LoadTexture("fern4"), true, true);
	TexturedModel tmFern1(mFern, mtFern1);
	TexturedModel tmFern2(mFern, mtFern2);
	TexturedModel tmFern3(mFern, mtFern3);
	TexturedModel tmFern4(mFern, mtFern4);
	for (int i = 0; i < 20; i++){
		int x = rand() % 200 - 100;
		int z = rand() % 200 - 100;
		entities.push_back(Entity(i < 5 ? tmFern1 : i < 10 ? tmFern2 : i < 15 ? tmFern3 : tmFern4, glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	}

	RawModel mCube = OBJLoader::LoadObjModel("cube", loader);
	ModelTexture mtCube(loader.LoadTexture("cube"), false, true);
	TexturedModel tmCube(mCube, mtCube, true);
	entities.push_back(Entity(tmCube, glm::vec3(-8, 1, 4), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	entities.push_back(Entity(tmCube, glm::vec3(-8, 3, 4), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	entities.push_back(Entity(tmCube, glm::vec3(-8, 5, 4), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));

	entities.push_back(Entity(tmCube, glm::vec3(13, 1, 18), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	entities.push_back(Entity(tmCube, glm::vec3(13, 3, 18), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	entities.push_back(Entity(tmCube, glm::vec3(13, 5, 18), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));

	entities.push_back(Entity(tmCube, glm::vec3(6, 1, -7), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	entities.push_back(Entity(tmCube, glm::vec3(6, 3, -7), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	entities.push_back(Entity(tmCube, glm::vec3(6, 5, -7), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	*/

	RawModel mFrog = OBJLoader::LoadObjModel("standing_frog", loader);
	ModelTexture mtFrog(loader.LoadTexture("frogTex"), true, true);
	TexturedModel tmFrog(mFrog, mtFrog, true);
	texModels.push_back(tmFrog);

	entities.push_back(Entity(tmFrog, glm::vec3(400, 1, 400), glm::vec3(0, 0, 0), glm::vec3(5, 5, 5), glm::vec3(0, 0, 1)));
	terrains.push_back(Terrain(-1, 0, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
	terrains.push_back(Terrain(0, 0, loader, ModelTexture(loader.LoadTexture("scape", true)), true));
	terrains.push_back(Terrain(-1, -1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
	terrains.push_back(Terrain(0, -1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
	terrains.push_back(Terrain(1, -1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
	terrains.push_back(Terrain(1, 0, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
	terrains.push_back(Terrain(1, 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
	terrains.push_back(Terrain(0, 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
	terrains.push_back(Terrain(-1, 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));

	
	FPSCamera camera(true);
	// END temporary data

	MasterRenderer renderer(m_displayManager->GetAspect());
	// Start the game loop
	while (m_displayManager->IsWindowOpen())
	{
		camera.Update();

		if (entities.size() > 200)
		{
			entities.erase(entities.begin(), entities.begin() + 100);
		}

		generateNewLand(camera.GetPosition(), terrains, entities, loader);


		simulate(dt);
		
		
		Light light(camera.GetPosition(), glm::vec3(1, 1, 1));

		for (Terrain& t : terrains)
			renderer.ProcessTerrain(t);
		for (Entity& e : entities)
			renderer.ProcessEntity(e);

		renderer.Render(light, camera);

		m_displayManager->UpdateDisplay();
		m_displayManager->ShowUPS();
	}
}

void GameManager::simulate(float dt)
{
	srand(time(0));
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> check(0, 100);
	std::uniform_real_distribution<> distr(-20, 20.0);



	for (Entity& e : entities)
	{

		//works fine as long as the frog moves forward

		if (check(eng) == 100)
		{

			glm::vec3 newPos = glm::vec3(distr(eng), 0.0, distr(eng));
			//take the front vector and find the angle between front and the new position

			//arccos(u dot v / (mag u * mag v))

			//or arcsin mag (u x v) / mag (u) * mag(v)

			//float angle = acos((glm::dot(e.GetFront(), glm::vec3(x, 0, z)) ) / (e.GetFront().length() * glm::vec3(x, 0, z).length()));

			

			//std::cout << glm::cross(e.GetFront(), newPos).length() << std::endl;
			//std::cout << e.GetFront()[0] << " " << e.GetFront()[1] << " " << e.GetFront()[2] << std::endl;

			double angle = acos(glm::dot(e.GetFront(), glm::normalize(newPos)));

			//std::cout << x << " " << z << std::endl;
			

			if (!isnan(angle))
			{
				//need to change the rotation correctly
				e.SetRotation(glm::vec3(0.0, angle, 0.0));
			}
			//e.SetFront(glm::normalize(newPos));

			//std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			e.ChangePosition(newPos);
		
	}
	}
}

void GameManager::generateEntities(glm::vec3 position, int minX, int minZ, int maxX, int maxZ)
{
	srand(time(0));
	//make this more consistent
	for (int i = 0; i < rand() % 64; i++)
	{
		float newX, newZ;

		newX = rand() % int(maxX + 1) + minX;
		newZ = rand() % int(maxZ + 1) + minZ;

		entities.push_back(Entity(texModels[0], glm::vec3(newX, 1, newZ), glm::vec3(0, 0, 0), glm::vec3(30, 30, 30), glm::vec3(0, 0, 1)));
	}


}

void GameManager::generateNewLand(glm::vec3 position, std::vector<Terrain>& terrains, std::vector<Entity>& entities, Loader &loader)
{
	std::vector<Terrain> temp;

	for (Terrain& terrain : terrains)
	{
		
		float x = position[0];
		float z = position[2];
		
		float size = terrain.GetSize();
		float halfsize = size / 2;

		//reduce to 2D; x-z plane; need to optimize
		float distToMid = sqrt(pow(x - (terrain.GetX() + halfsize), 2) + pow(z - (terrain.GetZ() + halfsize), 2));
		if (distToMid < 350 && !terrain.getMiddle())
		{

			//-1 0, 1, 0
			temp.push_back(Terrain((terrain.GetX() / size) - 1, (terrain.GetZ()/ size), loader, ModelTexture(loader.LoadTexture("scape", true)), false));
			temp.push_back(Terrain((terrain.GetX() / size) + 1, (terrain.GetZ() / size), loader, ModelTexture(loader.LoadTexture("scape", true)), false));

			//-1,1 0,1 1,1
			temp.push_back(Terrain((terrain.GetX() / size) - 1, (terrain.GetZ() / size) + 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
			temp.push_back(Terrain((terrain.GetX() / size), (terrain.GetZ() / size) + 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
			temp.push_back(Terrain((terrain.GetX() / size) + 1, (terrain.GetZ() / size) + 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));

			//-1,-1 0,-1, 1, -1

			float minX = ((terrain.GetX() / size) - 1) * size;
			float minZ = ((terrain.GetZ() / size) - 1) * size;

			float maxX = ((terrain.GetX() / size) + 1) * size;
			float maxZ = ((terrain.GetZ() / size) + 1) * size;

			temp.push_back(Terrain((terrain.GetX() / size) - 1, (terrain.GetZ() / size) - 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
			temp.push_back(Terrain((terrain.GetX() / size), (terrain.GetZ() / size) - 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
			temp.push_back(Terrain((terrain.GetX() / size) + 1, (terrain.GetZ() / size) - 1, loader, ModelTexture(loader.LoadTexture("scape", true)), false));

			terrain.setMiddle();
			temp.push_back(terrain);
			terrains.clear();

			srand(time(0));

			generateEntities(position, minX, minZ, maxX, maxZ);

		}

		for (Terrain& terrain : temp)
		{
			terrains.push_back(terrain);
		}
		
		temp.clear();
	}
}