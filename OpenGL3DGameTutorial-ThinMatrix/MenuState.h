#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "../OpenGL3DGameTutorial-ThinMatrix/GameState.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/RunState.h"
#include "../OpenGL3DGameTutorial-ThinMatrix/MenuItem.h"

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
	MenuState() {}

private:
	//for now lets just set up a vector of stuff to render
	std::vector<MenuItem> menuItems;
};

#endif // !MENUSTATE_H

