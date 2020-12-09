#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include "../Shaders/ShaderProgram.h"

class TextShader : public ShaderProgram
{
public:
	TextShader();
	void LoadModelMatrix(const glm::mat4& matrix);
	void LoadProjectionMatrix(const glm::mat4& matrix);
	void LoadViewMatrix(glm::mat4& cameraViewMatrix);
	void LoadPosition(glm::vec3& world_pos);
	GLuint getTextureLocation();
	GLuint getColorLocation();
	GLuint getBillboardLocation();
	~TextShader();
private:
	GLuint location_ModelMatrix;
	GLuint location_ProjectionMatrix;
	GLuint location_ViewMatrix;
	GLuint location_texture;
	GLuint location_textColor;
	GLuint location_billboard;
	GLuint location_world_pos;

protected:
	void BindAttributes();
	void GetAllUniformLocations();
};
#endif
