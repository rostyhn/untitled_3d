#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "TextRenderer.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/MenuItem.h"
#include "../Entities/FPSCamera.h"

class MasterRenderer
{
public:
	MasterRenderer(float aspectRatio);
	virtual ~MasterRenderer();

	void Render(Light& light, FPSCamera camera);
	void RenderMenu();
	void Process2DText(MenuItem& menuItem);
	void ProcessTerrain(Terrain& terrain);
	void ProcessEntity(Entity& entity);
private:
	const float m_FOV = 70;
	const float m_NEAR_PLANE = 0.1f;
	const float m_FAR_PLANE = 5000;
	glm::mat4 m_projectionMatrix;
	BasicShader m_basicShader;
	TerrainShader m_terrainShader;
	EntityRenderer m_entityRenderer;
	TerrainRenderer m_terrainRenderer;
	TextShader m_textShader;
	TextRenderer m_textRenderer;
	std::vector<Terrain> terrains;
	std::vector<MenuItem> menuItems;
	std::map<TexturedModel, std::vector<Entity>, tmCompare> m_entities;
};

#endif // MASTERRENDERER_H