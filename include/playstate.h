#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "gamestate.h"

class PlayState : public CGameState 
{
public:
  void Init(CGame* game);
  void Draw(CGame* game, float* delta);

  static PlayState* Instance() {
    return &m_PlayState;
  }
protected:
  PlayState() {}
private:
  static PlayState m_PlayState;
};
#endif