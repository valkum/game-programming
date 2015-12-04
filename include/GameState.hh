#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.hh"

class CGameState {
public:

  virtual void init(CGame *game) = 0;

  // void Cleanup();

  // Safe acutal buffer to be visible in background in PauseState
  // void Pause();
  // void Resume();

  virtual void handleKeyEvents(GLFWwindow *window,
                               int         key,
                               int         scancode,
                               int         action,
                               int         mods) {
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }
  }

  void handleResizeEvents(GLFWwindow *window, glm::uvec2 windowSize) {
    // store the new window size and adjust the viewport:
    glViewport(0, 0, windowSize.x, windowSize.y);
  }

  virtual void handleMouseMoveEvents(GLFWwindow *window,
                                     glm::vec2   mousePos) = 0;
  virtual void handleMouseButtonEvents(GLFWwindow *window,
                                      glm::vec2   mousePos,
                                       int         button,
                                       int         action,
                                       int         mods)   = 0;
  virtual void update(CGame *game,
                      float  delta)                        = 0;
  virtual void draw(CGame *game,
                    float *delta)                          = 0;

  void         changeState(CGame *game, CGameState *state) {
    game->changeState(state);
  }

protected:

  CGameState() {}
};

#endif // ifndef GAMESTATE_H
