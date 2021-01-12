#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "../RenderEngine/Loader.h"
#include <glm/glm.hpp>

struct Collidable {

  Collidable() {}
  
  Collidable(const char* modelName) {
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
    length = bBox[3] - bBox[0];
    height = bBox[4] - bBox[1];
    width  = bBox[5] - bBox[2];
  }

  Collidable(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    corners.push_back(glm::vec3(minX, minY, minZ)); //min
    corners.push_back(glm::vec3(minX, minY, maxZ));
    corners.push_back(glm::vec3(minX, maxY, minZ));
    corners.push_back(glm::vec3(maxX, minY, minZ));
    corners.push_back(glm::vec3(minX, maxY, maxZ));
    corners.push_back(glm::vec3(maxX, minY, maxZ)); 
    corners.push_back(glm::vec3(maxX, maxY, minZ));
    corners.push_back(glm::vec3(maxX, maxY, maxZ)); //max
    length = maxX - minX;
    height = maxY - minY;
    width =  maxZ - minZ;
  }
  
  std::vector<glm::vec3> corners;
  float height;
  float width;
  float length;
  bool isStatic;
};

#endif
