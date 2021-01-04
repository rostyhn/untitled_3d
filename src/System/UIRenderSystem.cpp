#include "UIRenderSystem.h"
#include "freetype-gl/freetype-gl.h"
#include "freetype-gl/vertex-buffer.h"
#include "ft2build.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include FT_FREETYPE_H

#include "../Toolbox/Coordinator.h"
#include "../Components/Transform.h"
#include "../Components/UIElement.h"

UIRenderSystem::UIRenderSystem()
    : m_shader("../res/shaders/textShader.vert",
               "../res/shaders/textShader.frag"),
      m_orthographic(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f)) {}

void UIRenderSystem::Init() {

  // switch to atlas
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
  }
  FT_Face face;
  if (FT_New_Face(ft, "../fonts/Vera.ttf", 0, &face)) {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
  }

  FT_Set_Pixel_Sizes(face, 0, 24);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

  for (unsigned char c = 0; c < 128; c++) {
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use

    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x,
    };
    characters.insert(std::pair<char, Character>(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void UIRenderSystem::Render() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_shader.use();
  Coordinator* coordinator = &Coordinator::GetInstance();
  m_shader.setMat4("projection", m_orthographic);
  
  for (const auto &e : m_Entities) {
    
    auto const& transform = coordinator->GetComponent<Transform>(e);
    auto& ui = coordinator->GetComponent<UIElement>(e);
    
    m_shader.setVec3("textColor", ui.color[0], ui.color[1], ui.color[2]);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    m_shader.setInt("isBillboard", 0);
    std::string::const_iterator c;

    float x = transform.position[0];
    float y = transform.position[1];

    if(ui.newUI) {
      ui.xMin = transform.position[0];
      ui.yMin = transform.position[1];
      ui.xMax = transform.position[0];
      ui.yMax = transform.position[1];
    }
    float maxH = 0.0f;
    
    for (c = ui.text.begin(); c != ui.text.end(); c++) {
      Character ch = characters[*c];
      float xpos = x + ch.Bearing.x * ui.scale;
      float ypos = y - (ch.Size.y - ch.Bearing.y) * ui.scale;
      float w = ch.Size.x * ui.scale;
      float h = ch.Size.y * ui.scale;
      if(ui.newUI) {
	if(xpos + w > ui.xMax) {
	  ui.xMax = xpos + w;
	}
	if(h > maxH) {
	  maxH = h;
	}
      }
      
      // update VBO for each character
      float vertices[6][5] = {{xpos, ypos + h, 0.0f, 0.0f, 0.0f},
                              {xpos, ypos, 0.0f, 0.0f, 1.0f},
                              {xpos + w, ypos, 0.0f, 1.0f, 1.0f},

                              {xpos, ypos + h, 0.0f, 0.0f, 0.0f},
                              {xpos + w, ypos, 0.0f, 1.0f, 1.0f},
                              {xpos + w, ypos + h, 0.0f, 1.0f, 0.0f}};
      // render glyph texture over quad
      glBindTexture(GL_TEXTURE_2D, ch.TextureID);
      // update content of VBO memory
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      // render quad
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // now advance cursors for next glyph (note that advance is number of 1/64
      // pixels)
      x += (ch.Advance >> 6) * ui.scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
   if(ui.newUI) {
      ui.yMax += maxH;
      ui.newUI = false;
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void UIRenderSystem::Update() {}
