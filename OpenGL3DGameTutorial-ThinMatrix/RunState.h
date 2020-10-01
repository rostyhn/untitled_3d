#ifndef RUNSTATE_H
#define RUNSTATE_H
#include "../OpenGL3DGameTutorial-ThinMatrix/GameState.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/RenderEngine/Loader.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Models/RawModel.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Models/TexturedModel.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Textures/ModelTexture.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Shaders/BasicShader.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Entities/Entity.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Entities/Camera.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Entities/FPSCamera.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/RenderEngine/OBJLoader.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/Terrain/Terrain.h"

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
