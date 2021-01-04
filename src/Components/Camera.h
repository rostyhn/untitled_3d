#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct Camera {
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 direction;
  glm::vec2 mousePosition;
  float sensitivity = 0.005f;

  glm::mat4 getView() const {
    return glm::lookAt(position, position + front, up);
  }
};
#endif
