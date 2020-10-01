#ifndef STATE_MANAGER
#define STATE_MANAGER
class StateManager
{
public:
	StateManager();
	~StateManager();

	void ChangeState(GameState* newState);

	GameState* GetActiveState() { return m_ActiveState; }

	void Draw();
};

private:
	GameState* m_ActiveState;
#endif

