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
	void Init(GLFWwindow* g_window, glm::vec2 _g_window_size);
	void Cleanup();

	void ChangeState(CGameState* state);

	void HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods);
	void HandleResizeEvents(GLFWwindow * window, int newWidth, int newHeight );
	void HandleMouseEvents(GLFWwindow* window, double xpos, double ypos);
	void Update();
	void Draw(float* delta);

	bool Running() { return m_running; }
	void Quit() {m_running = false; }
	GLFWwindow* g_window;
	glm::uvec2 g_windowSize;
private:
	std::stack<CGameState*> states;

	bool m_running;
	bool m_fullscreen;

};

#endif
