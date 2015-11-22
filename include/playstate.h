#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "gamestate.h"

class PlayState : public CGameState 
{
public:
    void Init(CGame* game);
    void Draw(CGame* game, float* delta);

    void HandleMouseMoveEvents(GLFWwindow* window, glm::vec2 mousePos);
    void HandleMouseButtonEvents(GLFWwindow* window, glm::vec2 mousePos, int button, int action, int mods);


    static PlayState* Instance() {
        return &m_PlayState;
    }
protected:
    PlayState() {}
private:
    static PlayState m_PlayState;
};
#endif