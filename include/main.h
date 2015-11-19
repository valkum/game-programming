#ifndef MAIN_H
#define MAIN_H

#include <ACGL/OpenGL/GL.hh>
#include <GLFW/glfw3.h>
#include <ACGL/OpenGL/Objects/ArrayBuffer.hh>

GLFWwindow* g_window;

int createWindow();
int run(char *argv[]);

#endif