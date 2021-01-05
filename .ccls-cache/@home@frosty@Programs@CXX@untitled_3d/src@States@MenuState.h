#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include "RunState.h"

//temp for now just to test that the menu system works 

class MenuState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameManager* pManager);
	void Update(GameManager* pManager);
	void Draw(GameManager* pManager);

	static MenuState& Instance() {
		static MenuState *m_MenuState = new MenuState();
		return *m_MenuState;
	}

protected:
   MenuState();
};

#endif // !MENUSTATE_H

