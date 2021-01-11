#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "../RenderEngine/Loader.h"
#include <glm/glm.hpp>

struct Collidable {

  Collidable() {}
  
  Collidable(const char* modelName, bool stat = false) {
    Loader* loader = &Loader::getInstance();
    std::vector<float> bBox = loader->GetBoundingBox(modelName);

    corners.push_back(glm::vec3(bBox[0], bBox[1], bBox[2])); //min
    corners.push_back(glm::vec3(bBox[0], bBox[1], bBox[5])); //minX, minY, maxZ
    corners.push_back(glm::vec3(bBox[0], bBox[4], bBox[2])); //minX, maxY, minZ
    corners.push_back(glm::vec3(bBox[3], bBox[1], bBox[2])); //maxX, minY, minZ
    corners.push_back(glm::vec3(bBox[0], bBox[4], bBox[5])); //minX, maxY, maxZ
    corners.push_back(glm::vec3(bBox[3], bBox[1], bBox[5])); //maxX, minY, maxZ
    corners.push_back(glm::vec3(bBox[3], bBox[4], bBox[2])); //maxX, maxY, minZ
    corners.push_back(glm::vec3(bBox[3], bBox[4], bBox[5])); //max
    isStatic = stat;
  }

  Collidable(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, bool stat = false) {
    corners.push_back(glm::vec3(minX, minY, minZ)); //min
    corners.push_back(glm::vec3(minX, minY, maxZ));
    corners.push_back(glm::vec3(minX, maxY, minZ));
    corners.push_back(glm::vec3(maxX, minY, minZ));
    corners.push_back(glm::vec3(minX, maxY, maxZ));
    corners.push_back(glm::vec3(maxX, minY, maxZ)); 
    corners.push_back(glm::vec3(maxX, maxY, minZ));
    corners.push_back(glm::vec3(maxX, maxY, maxZ)); //max
    isStatic = stat;
  }
  
  std::vector<glm::vec3> corners;
  bool isStatic;
};

#endif
