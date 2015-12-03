#include "IntroState.hh"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Base/Settings.hh>

#include "PlayState.hh"
#include <iostream>
#include "Helper.hh"
#include "GUI/Gui.hh"
#include "GUI/Button.hh"

using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;

IntroState  IntroState::m_IntroState;
Gui* gui;
NVGcontext* vg = nullptr;

class IntroState;
void IntroState::init(CGame *game) {


  gui = new Gui(vg, game->g_window);

  Button *playButton = new Button(gui, "Play");
  playButton->setCallback([&] {
    cout << "Button Play Clicked" << endl;
    CGame *g = CGame::instance();
    g->changeState(PlayState::instance());
  });
  playButton->setBackgroundColor(Color(0, 96, 128, 255));
  playButton->setPosition(ivec2(200, 170));
  playButton->setSize(ivec2(170, 28));

  // Button *closeButton = new Button(gui, "Play");
  // closeButton->setCallback([&] {
  //   cout << "Button Exit Clicked" << endl;
  //   CGame *g = CGame::instance();
  //   glfwSetWindowShouldClose(g->g_window, GL_TRUE);
  // });

  
}

void IntroState::draw(CGame *game, float *delta) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  gui->drawAll();

  // glfwGetCursorPos(game->g_window, &mx, &my);
  // glfwGetWindowSize(game->g_window, &winWidth, &winHeight);
  // glfwGetFramebufferSize(game->g_window, &fbWidth, &fbHeight);

  // // Calculate pixel ration for hi-dpi devices.
  // pxRatio = (float)fbWidth / (float)winWidth;

  // // glViewport(0,0, fbWidth, fbHeight);
  // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
  // drawButton(vg, 0, "Play", 200, 170, 140, 28, nvgRGBA(0, 96, 128, 255));
  // drawButton(vg, 0, "Exit", 200, 210, 140, 28, nvgRGBA(0, 0, 0, 0));

  // nvgEndFrame(vg);
}

void IntroState::handleKeyEvents(GLFWwindow *window,
                                 int         key,
                                 int         scancode,
                                 int         action,
                                 int         mods) {
  CGameState::handleKeyEvents(window, key, scancode, action, mods);
  gui->keyCallbackEvent(key, scancode, action, mods);

  if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS)) {
    CGame *g = CGame::instance();
    g->changeState(PlayState::instance());
  }
}

void IntroState::handleMouseButtonEvents(GLFWwindow *window,
                                         vec2        mousePos,
                                         int         button,
                                         int         action,
                                         int         mods) {
    gui->mouseButtonCallbackEvent( button, action == GLFW_PRESS, mods);
  
}

void IntroState::handleMouseMoveEvents(GLFWwindow *window, vec2 mousePos) {
  gui->cursorPosCallbackEvent(mousePos.x, mousePos.y);
}
