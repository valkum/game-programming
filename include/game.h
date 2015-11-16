#ifndef GAME_H
#define GAME_H
#include <stack>

class CGameState;

class CGame
{
public:
	void Init();
	void Cleanup();

	void ChangeState(CGameState* state);

	void HandleEvents();
	void Update();
	void Draw(float* delta);

	bool Running() { return m_running; }
	void Quit() {m_running = false; }
private:
	std::stack<CGameState*> states;

	bool m_running;
	bool m_fullscreen;

};

#endif
