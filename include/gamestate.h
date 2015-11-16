#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "game.h"

class CGameState
{
public:
	virtual void Init() =0;
	//void Cleanup();

	// Safe acutal buffer to be visible in background in PauseState
	//void Pause();
	//void Resume();

	//void HandleEvents(CGame* game );
	//void Update(CGame* game, float delta);
	virtual void Draw(CGame* game, float* delta) =0;

	void ChangeState(CGame* game, CGameState* state) {
		game->ChangeState(state);
	}

protected:
	CGameState() {}
};

#endif