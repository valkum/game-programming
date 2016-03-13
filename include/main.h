#ifndef MAIN_H
#define MAIN_H
#include "IntroState.hh"
#include "PlayState.hh"
#include "Game.hh"

GLFWwindow *g_window;

StartSettings cli_settings;
int createWindow();
int run();

// static void keyCallback(GLFWwindow* window, int key, int scancode, int
// action, int mods);
// static void resizeCallback(GLFWwindow * window, int newWidth, int newHeight);


#endif // ifndef MAIN_H
