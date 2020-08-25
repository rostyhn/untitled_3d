#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include "../Shaders/ShaderProgram.h"

class TextShader : public ShaderProgram
{
public:
	TextShader();
	void LoadModelMatrix(glm::mat4& matrix);
	void LoadProjectionMatrix(glm::mat4& matrix);
	void LoadViewMatrix(glm::mat4& cameraViewMatrix);
	GLuint getTextureLocation();
	GLuint getColorLocation();
	~TextShader();
private:
	GLuint location_ModelMatrix;
	GLuint location_ProjectionMatrix;
	GLuint location_ViewMatrix;
	GLuint location_texture;
	GLuint location_textColor;


protected:
	void BindAttributes();
	void GetAllUniformLocations();
};
#endif
