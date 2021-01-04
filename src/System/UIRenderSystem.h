#ifndef UIRENDERSYSTEM_H
#define UIRENDERSYSTEM_H

#include "System.h"
#include "../Shaders/Shader.h"
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>
#include <freetype-gl/vertex-buffer.h>
#include <freetype-gl/vector.h>
#include <glm/glm.hpp>
#include <map>

struct Character {
  unsigned int TextureID;  // ID handle of the glyph texture
  glm::ivec2   Size;       // Size of glyph
  glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
  unsigned int Advance;    // Offset to advance to next glyph
};

class UIRenderSystem : public System {
 public:
  UIRenderSystem();
  void Init();
  void Render();
  void Update();
  std::map<char, Character> characters;
 private:
  Shader m_shader;
  glm::mat4 m_orthographic;
  unsigned int VAO, VBO;
};
#endif
