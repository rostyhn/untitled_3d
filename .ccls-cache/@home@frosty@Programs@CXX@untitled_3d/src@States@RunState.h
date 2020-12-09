#ifndef RUNSTATE_H
#define RUNSTATE_H
#include "../src/States/GameState.h"
#include "../src/RenderEngine/Loader.h"
#include "../src/Models/RawModel.h"
#include "../src/Models/TexturedModel.h"
#include "../src/Textures/ModelTexture.h"
#include "../src/Shaders/BasicShader.h"
#include "../src/Entities/Entity.h"
#include "../src/Entities/Camera.h"
#include "../src/Entities/FPSCamera.h"
#include "../src/RenderEngine/OBJLoader.h"
#include "../src/Terrain/Terrain.h"

#include <random>
#include <chrono>
#include <thread>
#include <iostream>

class RunState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameManager* pManager);
	void Update(GameManager* pManager);
	void Draw(GameManager* pManager);

	static RunState& Instance() {
		static RunState *m_RunState = new RunState();
		return *m_RunState;
	}

protected:
	RunState() { }
private:
	float t;
	float dt;
	bool paused = false;
	void generateNewLand(glm::vec3 position, std::vector<Terrain>& terrains, std::vector<Entity>& entities, Loader &loader);
	void generateEntities(glm::vec3 position, int minX, int minZ, int maxX, int maxZ);
	void simulate(float dt);
	std::vector<Entity> entities;
	std::vector<Terrain> terrains;
	std::vector<TexturedModel> texModels;
	Loader loader;
	FPSCamera camera;
};
#endif // !RUNSTATE_H
