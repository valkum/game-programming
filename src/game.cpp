#include "game.h"
#include "gamestate.h"
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;
CGame CGame::m_CGame;
class CGame;

void CGame::Init(GLFWwindow* _g_window){
    g_window = _g_window;
    g_windowSize = glm::uvec2 ( 640, 480 );

	m_running = true;
	std::cout<<"Game Init\n"<<std::endl;
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	std::cout<<"Running against GLFW: "<<major<<"."<<minor<<"."<<revision<<std::endl;
	std::cout<<"OpenGL Version: "<<glGetString(GL_VERSION)<<std::endl;

    glfwSetMouseButtonCallback(g_window,
        [](GLFWwindow *w, int button, int action, int mods) {
            CGame* g = CGame::Instance();
            g->HandleMouseButtonEvents(w, button, action, mods);
        }
    );

    glfwSetCursorPosCallback(g_window,
        [](GLFWwindow *w, double x, double y) {
            CGame* g = CGame::Instance();
            g->HandleMouseMoveEvents(w, x, y);
        }
    );

    glfwSetKeyCallback(g_window,
        [](GLFWwindow *w, int key, int scancode, int action, int mods) {
            CGame* g = CGame::Instance();
            g->HandleKeyEvents(w, key, scancode, action, mods);
        }
    );
    glfwSetFramebufferSizeCallback(g_window,
        [](GLFWwindow *w, int width, int height) {
            CGame* g = CGame::Instance();
            g->HandleResizeEvents(w, width, height);
        }
    );

}
void CGame::Cleanup(){
    //@todo add cleanup to all states.
}
void CGame::ChangeState(CGameState* _game){
	states.push(_game);
	_game->Init(this);

}

void CGame::Draw(float* delta){
	states.top()->Draw(this, delta);
}
void CGame::HandleMouseButtonEvents(GLFWwindow* window, int button, int action, int mods ) {
    states.top()->HandleMouseButtonEvents(window, m_mousePos, button, action, mods);
}
void CGame::HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods){
    states.top()->HandleKeyEvents(window, key, scancode, action, mods);
}
void CGame::HandleMouseMoveEvents(GLFWwindow* window, double xpos, double ypos){
    m_mousePos = glm::vec2(xpos, ypos);
    states.top()->HandleMouseMoveEvents(window, m_mousePos);
}
void CGame::HandleResizeEvents(GLFWwindow * window, int width, int height){
    g_windowSize = glm::uvec2( width, height);
    states.top()->HandleResizeEvents(window, g_windowSize);
}