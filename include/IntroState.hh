#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "GameState.hh"

class IntroState : public CGameState {
public:

  void Init(CGame *game);
  void Draw(CGame *game,
            float *delta);
  void Update(CGame *game, float delta) {}

  void HandleKeyEvents(GLFWwindow *window,
                       int         key,
                       int         scancode,
                       int         action,
                       int         mods);
  void HandleMouseMoveEvents(GLFWwindow *window,
                             glm::vec2   mousePos);
  void HandleMouseButtonEvents(GLFWwindow *window,
                               glm::vec2   mousePos,
                               int         button,
                               int         action,
                               int         mods);
  void HandleResizeEvents(GLFWwindow *window,
                          glm::uvec2  windowSize);


  static IntroState* Instance() {
    return &m_IntroState;
  }

protected:

  IntroState() {}

private:

  static IntroState m_IntroState;
  int m_MouseState;
};
#endif // ifndef INTROSTATE_H
