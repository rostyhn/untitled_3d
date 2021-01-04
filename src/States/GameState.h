#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "../Engine/GameManager.h"
#include "../Toolbox/Coordinator.h"

class GameState
{
public:
	
	virtual void Init() = 0;
	virtual void Cleanup() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(GameManager* pManager) = 0;
	virtual void Update(GameManager* pManager) = 0;
	virtual void Draw(GameManager* pManager) = 0;
protected: 
  GameState() { }
  std::vector<Entity> m_Entities;

  
};


#endif // !GAMESTATE_H

