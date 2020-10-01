#include "RunState.h"

#include "../OpenGL3DGameTutorial-ThinMatrix/RenderEngine/MasterRenderer.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/MenuState.h"

/* TODO: implement terrain as chunks and delete anything that isn't present in the chunk
 * 
 */
void RunState::Init()
{
	srand(time(0));
	this->t = 0.0;
	this->dt = 0.01;
	RawModel mFrog = OBJLoader::LoadObjModel("standing_frog", loader);
	ModelTexture mtFrog(loader.LoadTexture("frogTex"), true, true);
	TexturedModel tmFrog(mFrog, mtFrog, true);
	texModels.push_back(tmFrog);

	entities.push_back(Entity(tmFrog, glm::vec3(400, 1, 400), glm::vec3(0, 0, 0), glm::vec3(5, 5, 5), glm::vec3(0, 0, 1)));
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			terrains.push_back(Terrain(i, j, loader, ModelTexture(loader.LoadTexture("scape", true)), false));
		}
	}
}

void RunState::Cleanup()
{
	terrains.clear();
	entities.clear();
}

void RunState::Pause()
{
	this->paused = true;
}

void RunState::Resume()
{
	this->paused = false;
}

void RunState::HandleEvents(GameManager * pManager)
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		DisplayManager* displayManager = pManager->m_displayManager;
		displayManager->toggleCursor();
		pManager->PushState(&MenuState::Instance());
	}
}

void RunState::Update(GameManager * pManager)
{
	camera.Update();
	generateNewLand(camera.GetPosition(), terrains, entities, loader);
	simulate(dt);
}

void RunState::Draw(GameManager* pManager)
{
	MasterRenderer* renderer = pManager->renderer;
	DisplayManager* displayManager = pManager->m_displayManager;
	
	if (entities.size() > 200)
	{
		entities.erase(entities.begin(), entities.begin() + 100);
	}
	Light light(camera.GetPosition(), glm::vec3(1, 1, 1));

	for (Terrain& t : terrains)
		renderer->ProcessTerrain(t);
	for (Entity& e : entities)
		renderer->ProcessEntity(e);

	renderer->Render(light, camera);
	displayManager->UpdateDisplay();
}


void RunState::simulate(float dt)
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

void RunState::generateEntities(glm::vec3 position, int minX, int minZ, int maxX, int maxZ)
{
	srand(time(0));
	//make this more consistent
	for (int i = 0; i < rand() % 10; i++)
	{
		float newX, newZ;

		newX = rand() % int(maxX + 1) + minX;
		newZ = rand() % int(maxZ + 1) + minZ;

		entities.push_back(Entity(texModels[0], glm::vec3(newX, 1, newZ), glm::vec3(0, 0, 0), glm::vec3(30, 30, 30), glm::vec3(0, 0, 1)));
	}
}

void RunState::generateNewLand(glm::vec3 position, std::vector<Terrain>& terrains, std::vector<Entity>& entities, Loader &loader)
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
			temp.push_back(Terrain((terrain.GetX() / size) - 1, (terrain.GetZ() / size), loader, ModelTexture(loader.LoadTexture("scape", true)), false));
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