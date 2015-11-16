#include <GLFW/glfw3.h>
#include "game.h"
#include "gamestate.h"
#include <iostream>

class CGame;

void CGame::Init(){
	m_running = true;
	std::cout<<"Game Init\n"<<std::endl;
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	std::cout<<"Running against GLFW"<<major<<"."<<minor<<"."<<revision<<std::endl;
	std::cout<<glGetString(GL_VERSION)<<std::endl;
}
void CGame::Cleanup(){

}
void CGame::ChangeState(CGameState* game){
	states.push(game);
	game->Init();

}

void CGame::Draw(float* delta){
	states.top()->Draw(this, delta);
}

