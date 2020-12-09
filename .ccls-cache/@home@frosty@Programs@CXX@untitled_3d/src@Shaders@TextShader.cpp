#include "TextShader.h"



TextShader::TextShader() : ShaderProgram("../res/shaders/textShader")
{
	BindAttributes();
	GetAllUniformLocations();
}

void TextShader::LoadModelMatrix(const glm::mat4 & matrix)
{
	LoadMatrix4(location_ModelMatrix, matrix);
}

void TextShader::LoadProjectionMatrix(const glm::mat4 & matrix)
{
	LoadMatrix4(location_ProjectionMatrix, matrix);
}

void TextShader::LoadViewMatrix(glm::mat4 & cameraViewMatrix)
{
	LoadMatrix4(location_ViewMatrix, cameraViewMatrix);
}

void TextShader::LoadPosition(glm::vec3 & world_pos)
{
	LoadVector(location_world_pos, world_pos);
}

GLuint TextShader::getTextureLocation()
{
	return location_texture;
}

GLuint TextShader::getColorLocation()
{
	return location_textColor;
}

GLuint TextShader::getBillboardLocation()
{
	return location_billboard;
}


TextShader::~TextShader()
{
}

void TextShader::BindAttributes()
{
	BindAttribute(0, "vertex");
	BindAttribute(1, "texture");
}

void TextShader::GetAllUniformLocations()
{
	location_ModelMatrix = GetUniformLocation("model");
	location_ProjectionMatrix = GetUniformLocation("projection");
	location_ViewMatrix = GetUniformLocation("view");
	location_texture = GetUniformLocation("texture");
	location_textColor = GetUniformLocation("textColor");
	location_billboard = GetUniformLocation("isBillboard");
	location_world_pos = GetUniformLocation("world_pos");
}
