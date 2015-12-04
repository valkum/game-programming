#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "GameState.hh"

class IntroState : public CGameState {
public:

  void init(CGame *game);
  void draw(CGame *game,
            float *delta);
  void update(CGame *game, float delta) {}

  void handleKeyEvents(GLFWwindow *window,
                       int         key,
                       int         scancode,
                       int         action,
                       int         mods);
  void handleMouseMoveEvents(GLFWwindow *window,
                             glm::vec2   mousePos);
  void handleMouseButtonEvents(GLFWwindow *window,
                               glm::vec2   mousePos,
                               int         button,
                               int         action,
                               int         mods);
  void handleResizeEvents(GLFWwindow *window,
                          glm::uvec2  windowSize);


  static IntroState* instance() {
    return &m_IntroState;
  }

protected:

  IntroState() {}

private:

  static IntroState m_IntroState;
  int m_MouseState;
};
#endif // ifndef INTROSTATE_H
