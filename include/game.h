#ifndef GAME_H
#define GAME_H
#include <ACGL/OpenGL/GL.hh>
#include <GLFW/glfw3.h>

#include <stack>
#include <glm/vec2.hpp>

class CGameState;

class CGame
{
public:
	void Init(GLFWwindow* g_window);
	void Cleanup();

	void ChangeState(CGameState* state);

	void HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods);
	void HandleResizeEvents(GLFWwindow * window, int width, int height);
	void HandleMouseButtonEvents(GLFWwindow* window, int button, int action, int mods);
	void HandleMouseMoveEvents(GLFWwindow* window, double x, double y);
	void Update();
	void Draw(float* delta);

	bool Running() { return m_running; }
	void Quit() {m_running = false; }
	GLFWwindow* g_window;
	glm::uvec2 g_windowSize;

	static CGame* Instance() {
		return &m_CGame;
	}

protected:
	CGame() {}
private:
	bool m_running;
	bool m_fullscreen;
	glm::vec2 m_mousePos;
	std::stack<CGameState*> states;

	static CGame m_CGame;
};

#endif
