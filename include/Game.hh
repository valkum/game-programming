#ifndef GAME_H
#define GAME_H
#include <ACGL/OpenGL/GL.hh>
#include <GLFW/glfw3.h>

#include <stack>
#include <glm/vec2.hpp>

class CGameState;

class CGame {
public:

  void init(GLFWwindow *g_window);
  void cleanup();

  void changeState(CGameState *state);

  void handleKeyEvents(GLFWwindow *window,
                       int         key,
                       int         scancode,
                       int         action,
                       int         mods);
  void handleResizeEvents(GLFWwindow *window,
                          int         width,
                          int         height);
  void handleMouseButtonEvents(GLFWwindow *window,
                               int         button,
                               int         action,
                               int         mods);
  void handleMouseMoveEvents(GLFWwindow *window,
                             double      x,
                             double      y);
  void update(float delta);
  void draw(float *delta);

  bool running() {
    return m_running;
  }

  void quit() {
    m_running = false;
  }

  GLFWwindow *g_window;
  glm::uvec2  g_windowSize;

  static CGame* instance() {
    return &m_CGame;
  }

protected:

  CGame() {}

private:

  bool m_running;

  // bool m_fullscreen;
  glm::vec2 m_mousePos;
  std::stack<CGameState *> states;

  static CGame m_CGame;
};

#endif // ifndef GAME_H
