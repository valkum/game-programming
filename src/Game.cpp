#include "Game.hh"
#include "GameState.hh"
#include <iostream>
#include <glm/glm.hpp>
#include <ACGL/Utils/Log.hh>

using namespace ACGL::Utils;
using namespace glm;
CGame CGame::m_CGame;
class CGame;

void CGame::init(GLFWwindow *_g_window) {
  g_window     = _g_window;
  g_windowSize = glm::uvec2(640, 480);

  m_running = true;
  std::cout << "Game Init\n" << std::endl;
  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  std::cout << "Running against GLFW: " << major << "." << minor << "." <<
  revision << std::endl;
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  glfwSetMouseButtonCallback(g_window,
                             [](GLFWwindow *w, int button, int action, int mods) {
    CGame *g = CGame::instance();
    g->handleMouseButtonEvents(w, button, action, mods);
  }
                             );

  glfwSetCursorPosCallback(g_window,
                           [](GLFWwindow *w, double x, double y) {
    CGame *g = CGame::instance();
    g->handleMouseMoveEvents(w, x, y);
  }
                           );

  glfwSetKeyCallback(g_window,
                     [](GLFWwindow *w,
                        int key,
                        int scancode,
                        int action,
                        int mods) {
    CGame *g = CGame::instance();
    g->handleKeyEvents(w, key, scancode, action, mods);
  }
                     );
  glfwSetFramebufferSizeCallback(g_window,
                                 [](GLFWwindow *w, int width, int height) {
    CGame *g = CGame::instance();
    g->handleResizeEvents(w, width, height);
  }
                                 );
}

void CGame::cleanup() {
  // @todo add cleanup to all states.
}

void CGame::changeState(CGameState *_game) {
  states.push(_game);
  _game->init(this);
}

void CGame::draw(float *extrapolation) {
  states.top()->draw(this, extrapolation);
}

void CGame::update(float dt) {
  //debug()<<std::to_string(dt)<<std::endl;
  states.top()->update(this, dt);
}

void CGame::handleMouseButtonEvents(GLFWwindow *window,
                                    int         button,
                                    int         action,
                                    int         mods) {
  states.top()->handleMouseButtonEvents(window, m_mousePos, button, action, mods);
}

void CGame::handleKeyEvents(GLFWwindow *window,
                            int         key,
                            int         scancode,
                            int         action,
                            int         mods) {
  states.top()->handleKeyEvents(window, key, scancode, action, mods);
}

void CGame::handleMouseMoveEvents(GLFWwindow *window, double xpos, double ypos) {
  m_mousePos = glm::vec2(xpos, ypos);
  states.top()->handleMouseMoveEvents(window, m_mousePos);
}

void CGame::handleResizeEvents(GLFWwindow *window, int width, int height) {
  g_windowSize = glm::uvec2(width, height);
  states.top()->handleResizeEvents(window, g_windowSize);
}
