#include "Engine/GameManager.h"
#include "../src/States/RunState.h"
#include <iostream>

int main(int argc, char** argv)
{
	GameManager gameManager;

	gameManager.Init();
	
	gameManager.ChangeState(&RunState::Instance());

	while (gameManager.Running()) {
		gameManager.HandleEvents();
		gameManager.Update();
		gameManager.Draw();
	}
	
	gameManager.Cleanup();

	return 0;
}
