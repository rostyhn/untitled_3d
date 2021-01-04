#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <string>
#include <glm/glm.hpp>

enum MenuCommand {
  CONT,
  QUIT
};

struct UIElement {
  std::string text;
  float scale;
  glm::vec3 color;
  MenuCommand command;
  float xMin, yMin, xMax, yMax;
  bool newUI = true;
};
#endif
