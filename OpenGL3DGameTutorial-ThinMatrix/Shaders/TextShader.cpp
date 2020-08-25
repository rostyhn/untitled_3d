#include "TextShader.h"



TextShader::TextShader() : ShaderProgram("../res/shaders/textShader")
{
	BindAttributes();
	GetAllUniformLocations();
}

void TextShader::LoadModelMatrix(glm::mat4 & matrix)
{
	LoadMatrix4(location_ModelMatrix, matrix);
}

void TextShader::LoadProjectionMatrix(glm::mat4 & matrix)
{
	LoadMatrix4(location_ProjectionMatrix, matrix);
}

void TextShader::LoadViewMatrix(glm::mat4 & cameraViewMatrix)
{
	LoadMatrix4(location_ViewMatrix, cameraViewMatrix);
}

GLuint TextShader::getTextureLocation()
{
	return location_texture;
}

GLuint TextShader::getColorLocation()
{
	return location_textColor;
}


TextShader::~TextShader()
{
}

void TextShader::BindAttributes()
{
	BindAttribute(0, "vertex");
	BindAttribute(1, "tex_coord");
	BindAttribute(2, "color");
}

void TextShader::GetAllUniformLocations()
{
	location_ModelMatrix = GetUniformLocation("model");
	location_ProjectionMatrix = GetUniformLocation("projection");
	location_ViewMatrix = GetUniformLocation("view");
	location_texture = GetUniformLocation("texture");
	location_textColor = GetUniformLocation("textColor");

}
