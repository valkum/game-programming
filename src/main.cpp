#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "introstate.h"
#include "game.h"


int main(int argc, char *argv[])
{
    run();
    

}
int createWindow() {
/* Initialize the library */
    if (!glfwInit())
        return false;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    /* Create a windowed mode window and its OpenGL context */
    g_window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!g_window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);
    return true;
}

int run() {
    if ( !createWindow() ) {
        glfwTerminate();
        exit( -1 );
    }
    CGame game;
    game.Init();
    game.ChangeState(IntroState::Instance());

    const int TICKS_PER_SECOND = 64;
    const float frameTime = 1 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 5;

    double newTime = glfwGetTime();
    int loops;
    float extrapolation;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window))
    {
        loops = 0;
        while( glfwGetTime() > newTime && loops < MAX_FRAMESKIP) {
            /* Poll for and process events */
            glfwPollEvents();
            //game.Update(frameTime);

            newTime += frameTime;
            loops++;
        }

        extrapolation = glfwGetTime() - (newTime - frameTime);
        //glfwGetFramebufferSize(g_window, &width, &height);
        game.Draw( &extrapolation );
        glfwSwapBuffers(g_window);

        
    }

    glfwTerminate();
    return true;
}

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
