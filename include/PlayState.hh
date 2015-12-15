#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.hh"

#include <ACGL/OpenGL/GL.hh>
#include <GLFW/glfw3.h>

#include <ACGL/OpenGL/Creator/VertexArrayObjectCreator.hh>

using namespace ACGL::OpenGL;

class PlayState : public CGameState {
public:

  void init(CGame *game);
  void draw(CGame *game,
            float *delta);
  void update(CGame *game,
              float  delta);

  void handleKeyEvents(GLFWwindow *window,
                       int         key,
                       int         scancode,
                       int         action,
                       int         mods);
  void handleResizeEvents(GLFWwindow *window,
                          glm::uvec2  windowSize);
  void handleMouseMoveEvents(GLFWwindow *window,
                             glm::vec2   mousePos);
  void handleMouseButtonEvents(GLFWwindow *window,
                               glm::vec2   mousePos,
                               int         button,
                               int         action,
                               int         mods);


  static PlayState* instance() {
    return &m_PlayState;
  }

protected:

  PlayState() {}

private:

  static PlayState m_PlayState;
  bool renderDebug;
  SharedShaderProgram skyboxShader;
  SharedShaderProgram cubeShader;
  SharedShaderProgram cloudShader;
  SharedArrayBuffer   debug_ab;
  SharedVertexArrayObject debug_vao;
  SharedShaderProgram     debugShader;
};
#endif // ifndef PLAYSTATE_H
