#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include <glm/glm.hpp>

enum CollidableType {
  BoundingBox
};

struct Collidable {

  Collidable() {}
  
  Collidable(CollidableType ct, bool stat = false) {
    collidableType = ct;
    isStatic = stat;
  };
  CollidableType collidableType;
  bool isStatic;
};

#endif
