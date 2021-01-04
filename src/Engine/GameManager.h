#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <GL/glew.h>
#include "../RenderEngine/DisplayManager.h"
#include <vector>

class GameState;

class GameManager {
public:
  void Init();
  void Cleanup();
  void ChangeState(GameState *state);
  void PushState(GameState *state);
  void PopState();
  void HandleEvents();
  void Update();
  void Draw();
  bool Running() { return m_running; }
  void Quit() { m_running = false; }
  bool m_keys[1024];
  bool m_mouse_buttons[8];
  double m_mouseX, m_mouseY;
  float resX, resY;
private:
  std::vector<GameState *> states;
  bool m_running;
};

#endif // GAMEMANAGER_H
