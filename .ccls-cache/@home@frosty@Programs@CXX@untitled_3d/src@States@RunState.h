#ifndef RUNSTATE_H
#define RUNSTATE_H
#include "GameState.h"
#include "../Engine/GameManager.h"

#include <random>
#include <chrono>
#include <thread>
#include <iostream>

class RunState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameManager* pManager);
	void Update(GameManager* pManager);
	void Draw(GameManager* pManager);

	static RunState& Instance() {
		static RunState *m_RunState = new RunState();
		return *m_RunState;
	}

protected:
  RunState();
private:
  bool paused = false;
  
  //eventually, https://gafferongames.com/post/fix_your_timestep/
  double t = 0.0;
  const double dt = 0.01;
};
#endif // !RUNSTATE_H
