#ifndef UICOLLISIONSYSTEM_H
#define UICOLLISIONSYSTEM_H
#include <glm/glm.hpp>
#include "System.h" 
#include "../Engine/GameManager.h"

class UICollisionSystem : public System { 
public: 
UICollisionSystem() {} 
 void Update(const glm::vec2 mousePos, const bool* mouseButtons, GameManager* pManager); 
 };
#endif
