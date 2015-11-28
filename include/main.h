#ifndef MAIN_H
#define MAIN_H
#include "IntroState.hh"
#include "Game.hh"

GLFWwindow *g_window;


int createWindow();
int run(char *argv[]);

// static void keyCallback(GLFWwindow* window, int key, int scancode, int
// action, int mods);
// static void resizeCallback(GLFWwindow * window, int newWidth, int newHeight);


#endif // ifndef MAIN_H
