#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "../Shaders/TextShader.h"
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>
#include <freetype-gl/vertex-buffer.h>
#include <freetype-gl/vector.h>
#include <GLFW/glfw3.h>
#include <map>

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(const TextShader& shader, const glm::mat4& projectionMatrix);
	~TextRenderer();
	void Render2D(TextShader &s, std::string text, float x, float y, float scale, glm::vec3 color);
	void Render3D(TextShader &s, std::string text, glm::vec3 location, float scale, glm::vec3 color);
	std::map<char, Character> characters;
	unsigned int VAO, VBO;

private:
	TextShader m_shader;
	void init();
};
#endif
