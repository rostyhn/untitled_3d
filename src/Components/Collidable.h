#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "../RenderEngine/Loader.h"
#include "AABB.h"
#include <glm/glm.hpp>
#include <math.h>

struct Collidable {

  //holds a reference to a model's exact AABB without any transforms
  AABB aabb;
  bool isStatic;
  
  Collidable() {}
  
  Collidable(const char* modelName, bool stat) {
    Loader* loader = &Loader::getInstance();
    aabb = loader->GetBoundingBox(modelName);
    isStatic = stat;
  }

  Collidable(float minX, float minY, float minZ,
	     float maxX, float maxY, float maxZ,
	     bool stat) {
    aabb = { minX, minY, minZ, maxX, maxY, maxZ };
    isStatic = stat;
  }
};

#endif
