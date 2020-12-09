#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <string>
#include <GLFW/glfw3.h>

class DisplayManager
{
public:
  static DisplayManager& getInstance() {
    static DisplayManager *m_instance = new DisplayManager(1920,1080,"untitled");
    return *m_instance;
  }
	virtual ~DisplayManager();
	bool IsWindowOpen();
	void UpdateDisplay();
	float& GetAspect();
	void ShowUPS();
	bool toggleCursor();
	GLFWwindow* getWindow() const;
protected:
        DisplayManager();
	DisplayManager(int width, int height, const std::string& title);
private:
        GLFWwindow* m_window;
	float m_aspect;
	std::string m_title;
};

#endif // DISPLAYMANAGER_H
