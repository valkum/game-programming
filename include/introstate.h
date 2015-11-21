#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "gamestate.h"

class IntroState : public CGameState 
{
public:
	void Init(CGame* game);
	void Draw(CGame* game, float* delta);

	static IntroState* Instance() {
		return &m_IntroState;
	}
protected:
	IntroState() {}
private:
	static IntroState m_IntroState;
};
#endif