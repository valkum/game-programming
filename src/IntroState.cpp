#include "IntroState.hh"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Base/Settings.hh>

#include "PlayState.hh"
#include <iostream>
#include "Helper.hh"
#include "GUI/Gui.hh"

using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;

IntroState  IntroState::m_IntroState;

class IntroState;
void IntroState::init(CGame *game) {
  glfwSetInputMode(game->g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glClearColor(1.0, 1.0, 1.0, 1.0);

  gui = new Gui(vg, game->g_window);

  Text *title = new Text(gui, "HIMMEL");
  title->setPosition(ivec2(200, 50));
  title->setFontSize(55.f);
  title->setTextColor(vec4(0.75f,0.75f,0.75f, 1.0f));

  Text *copyright = new Text(gui, "Copyright by Rudi Floren, Sebastian Bockholt and Kim Brose");
  copyright->setPosition(ivec2(200, 80));
  copyright->setTextColor(vec4(0.75f,0.75f,0.75f, 1.0f));

  Button *playButton1 = new Button(gui, "Play Level 1");
  playButton1->setCallback([&] {
    CGame *g = CGame::instance();
    g->cli_settings.levelId = "00000-00001";
    g->changeState(PlayState::instance());
  });
  playButton1->setBackgroundColor(Color(230, 230, 230, 255));
  playButton1->setPosition(ivec2(20, 170));
  playButton1->setTextColor(Color(40, 40, 40, 255));
  playButton1->setSize(ivec2(170, 50));

  Button *playButton2 = new Button(gui, "Play Level 2");
  playButton2->setCallback([&] {
    CGame *g = CGame::instance();
    g->cli_settings.levelId = "00000-00002";
    g->changeState(PlayState::instance());
  });
  playButton2->setBackgroundColor(Color(230, 230, 230, 255));
  playButton2->setPosition(ivec2(20, 230));
  playButton2->setTextColor(Color(40, 40, 40, 255));
  playButton2->setSize(ivec2(170, 50));

  Button *playButton3 = new Button(gui, "Play Level 3");
  playButton3->setCallback([&] {
    CGame *g = CGame::instance();
    g->cli_settings.levelId = "00000-00003";
    g->changeState(PlayState::instance());
  });
  playButton3->setBackgroundColor(Color(230, 230, 230, 255));
  playButton3->setPosition(ivec2(20, 290));
  playButton3->setTextColor(Color(40, 40, 40, 255));
  playButton3->setSize(ivec2(170, 50));

  Button *closeButton = new Button(gui, "Return to Desktop");
  closeButton->setCallback([&] {
    CGame *g = CGame::instance();
    glfwSetWindowShouldClose(g->g_window, GL_TRUE);
  });
  closeButton->setBackgroundColor(Color(100, 0, 0, 255));
  closeButton->setPosition(ivec2(20, 350));
  closeButton->setSize(ivec2(170, 50));
  
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
    g->cli_settings.levelId = "00000-00001";
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
