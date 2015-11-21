#include "game.h"
#include "gamestate.h"
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;
class CGame;



void CGame::Init(GLFWwindow* _g_window, vec2 _g_windowSize){
    g_window = _g_window;
    g_windowSize = _g_windowSize;

	m_running = true;
	std::cout<<"Game Init\n"<<std::endl;
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	std::cout<<"Running against GLFW: "<<major<<"."<<minor<<"."<<revision<<std::endl;
	std::cout<<"OpenGL Version: "<<glGetString(GL_VERSION)<<std::endl;
}
void CGame::Cleanup(){

}
void CGame::ChangeState(CGameState* _game){
	states.push(_game);
	_game->Init(this);

}

void CGame::Draw(float* delta){
	states.top()->Draw(this, delta);
}

void CGame::HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods){
    states.top()->HandleKeyEvents(this, window, key, scancode, action, mods);
}
void CGame::HandleMouseEvents(GLFWwindow* window, double xpos, double ypos){
    states.top()->HandleMouseEvents(this, window, xpos, ypos);
}
void CGame::HandleResizeEvents(GLFWwindow * window, int newWidth, int newHeight ){
    states.top()->HandleResizeEvents(this, window, newWidth, newHeight);
}