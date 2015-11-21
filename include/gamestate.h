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

	void HandleKeyEvents(CGame* game, GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        	glfwSetWindowShouldClose( window, GL_TRUE );
    	}
	}
	void HandleResizeEvents(CGame* game, GLFWwindow * window, int newWidth, int newHeight ) {
		// store the new window size and adjust the viewport:
	    game->g_windowSize = glm::uvec2( newWidth, newHeight);
	    glViewport( 0, 0, game->g_windowSize.x, game->g_windowSize.y );
	}
	void HandleMouseEvents(CGame* game, GLFWwindow* window, double xpos, double ypos);
	//void Update(CGame* game, float delta);
	virtual void Draw(CGame* game, float* delta) =0;

	void ChangeState(CGame* game, CGameState* state) {
		game->ChangeState(state);
	}

protected:
	CGameState() {}
};

#endif