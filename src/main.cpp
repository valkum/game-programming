#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Utils/Log.hh>
#include <ACGL/OpenGL/glloaders/extensions.hh>


using namespace std;
using namespace glm;
using namespace ACGL;
using namespace ACGL::Utils;

int main(int argc, char *argv[])
{
  // @todo: add loadLevel and version maybe
  run(argv);
}

void setGLFWHintsForOpenGLVersion(unsigned int _version)
{
#ifdef __APPLE__
# if (ACGL_OPENGL_VERSION >= 30)

  // request OpenGL 3.2, will return a 4.1 context on Mavericks
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
# endif // if (ACGL_OPENGL_VERSION >= 30)
#else // ifdef __APPLE__

  // non-apple
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _version / 10);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _version % 10);
    # ifdef ACGL_OPENGL_PROFILE_CORE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    # endif // ifdef ACGL_OPENGL_PROFILE_CORE
#endif // ifdef __APPLE__
}

int createWindow() {
  /* Initialize the library */
  if (!glfwInit()) return false;

  /////////////////////////////////////////////////////////////////////////////////////
  // Configure OpenGL context
  //
  setGLFWHintsForOpenGLVersion(ACGL_OPENGL_VERSION);

  // activate multisampling (second parameter is the number of samples):
  // glfwWindowHint( GLFW_SAMPLES, 8 );

  // request an OpenGL debug context:
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  /* Create a windowed mode window and its OpenGL context */

  g_window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

  if (!g_window)
  {
    glfwTerminate();
    error() << "Error creating window!" << endl;
    return false;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(g_window);
  ACGL::init();
  return true;
}

int run(char *argv[]) {
  if (!createWindow()) {
    glfwTerminate();
    exit(-1);
  }

  vector<std::string> tmp;
  tmp.push_back(std::string(argv[0]));
  glfwSetWindowTitle(g_window, tmp[tmp.size() - 1].c_str());
  CGame *game = CGame::instance();
  game->init(g_window);
  game->changeState(IntroState::instance());

  // Ensure we can capture the escape key being pressed below
  // glfwSetInputMode( g_window, GLFW_STICKY_KEYS, 1 );
  // glfwSetWindowSizeCallback(  g_window, resizeCallback );
  // glfwSetKeyCallback( g_window, keyCallback );

  const int   TICKS_PER_SECOND = 64;
  const float frameTime        = 1.0 / TICKS_PER_SECOND;
  const int   MAX_FRAMESKIP    = 5;

  double newTime = glfwGetTime();
  int    loops;
  float  extrapolation;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(g_window))
  {
    loops = 0;
    double now = glfwGetTime();

    while (glfwGetTime() > newTime && loops < MAX_FRAMESKIP) {
      /* Poll for and process events */
      glfwPollEvents();
      game->update(frameTime);

      newTime += frameTime;
      loops++;
    }

    extrapolation = glfwGetTime() - (newTime - frameTime);

    // glfwGetFramebufferSize(g_window, &width, &height);
    game->draw(&extrapolation);

    // @todo betterFPS meter? maybe draw in context?
    std::stringstream sstream(std::stringstream::in | std::stringstream::out);
    sstream << std::setprecision(1) << std::fixed
            << tmp[tmp.size() - 1] << " - FPS: " << 1 / (glfwGetTime() - now) <<
    " " << 1000 * (glfwGetTime() - now) / 1 << " msec";
    glfwSetWindowTitle(g_window, sstream.str().c_str());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwSwapBuffers(g_window);
  }

  glfwTerminate();
  return true;
}

// static void keyCallback(GLFWwindow* window, int key, int scancode, int
// action, int mods) {
//     game.HandleKeyEvents(window, key, scancode, action, mods);
// };
// static void resizeCallback(GLFWwindow * window, int newWidth, int newHeight )
// {
//     game.HandleResizeEvents(window, newWidth, newHeight);
// };

/**
   function loopyLoopExtra() {
    const int TICKS_PER_SECOND = 64;
    const float frameTime = 1 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 5;

    double newTime = glfwGetTime();
    int loops;
    float extrpolation;

    bool game_is_running = true;
    while( game_is_running ) {

        loops = 0;
        while( glfwGetTime() > newtime && loops < MAX_FRAMESKIP) {
            update_game(frameTime);

            newtime += frameTime;
            loops++;
        }

        extrapolation = glfwGetTime() - (newTime - frameTime);
        draw( extrapolation );

        //view_position = position + (speed * interpolation)
    }
   }
 */
