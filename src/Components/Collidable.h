#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "../RenderEngine/Loader.h"

struct Collidable {

  Collidable() {};
  Collidable(const char* modelName) {
    Loader* loader = &Loader::getInstance();
    std::vector<float> bBox = loader->GetBoundingBox(modelName);

    minX = bBox[0];
    minY = bBox[1];
    minZ = bBox[2];
    maxX = bBox[3];
    maxY = bBox[4];
    maxZ = bBox[5];
  }

  Collidable(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax) {
    minX = xMin;
    minY = yMin;
    minZ = zMin;

    maxX = xMax;
    maxY = yMax;
    maxZ = zMax;
  }
  
  float minX, minY, minZ, maxX, maxY, maxZ;
};

#endif
