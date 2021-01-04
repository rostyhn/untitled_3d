#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H
#include <glm/glm.hpp>
#include "System.h"

class ControlSystem : public System {
public:
  ControlSystem() {}
  void Update(const bool* keys, const glm::vec2 mousePos);
};
#endif
