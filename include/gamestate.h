#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "game.h"

class CGameState
{
public:
	virtual void Init(CGame* game) =0;
	//void Cleanup();

	// Safe acutal buffer to be visible in background in PauseState
	//void Pause();
	//void Resume();

	virtual void HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        	glfwSetWindowShouldClose( window, GL_TRUE );
    	}
	};
	void HandleResizeEvents(GLFWwindow * window, glm::uvec2 windowSize) {
		// store the new window size and adjust the viewport:
        glViewport( 0, 0, windowSize.x, windowSize.y );
	};
	virtual void HandleMouseMoveEvents(GLFWwindow* window, glm::vec2 mousePos) =0;
	virtual void HandleMouseButtonEvents(GLFWwindow* window, glm::vec2 mousePos, int button, int action, int mods)=0;
	virtual void Update(CGame* game, float delta) =0;
	virtual void Draw(CGame* game, float* delta) =0;

	void ChangeState(CGame* game, CGameState* state) {
		game->ChangeState(state);
	}

protected:
	CGameState() {}
};

#endif