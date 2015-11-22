#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "gamestate.h"

class IntroState : public CGameState 
{
public:
	void Init(CGame* game);
	void Draw(CGame* game, float* delta);

    void HandleMouseMoveEvents(GLFWwindow* window, glm::vec2 mousePos);
    void HandleMouseButtonEvents(GLFWwindow* window, glm::vec2 mousePos, int button, int action, int mods);


	static IntroState* Instance() {
		return &m_IntroState;
	}
protected:
	IntroState() {}
private:
	static IntroState m_IntroState;
    int m_MouseState;
};
#endif