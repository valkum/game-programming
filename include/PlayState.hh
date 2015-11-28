#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.hh"

#include <ACGL/OpenGL/GL.hh>
#include <GLFW/glfw3.h>

#include <ACGL/OpenGL/Creator/VertexArrayObjectCreator.hh>

using namespace ACGL::OpenGL;

class PlayState : public CGameState {
public:

  void Init(CGame *game);
  void Draw(CGame *game,
            float *delta);
  void Update(CGame *game,
              float  delta);

  void HandleKeyEvents(GLFWwindow *window,
                       int         key,
                       int         scancode,
                       int         action,
                       int         mods);
  void HandleResizeEvents(GLFWwindow *window,
                          glm::uvec2  windowSize);
  void HandleMouseMoveEvents(GLFWwindow *window,
                             glm::vec2   mousePos);
  void HandleMouseButtonEvents(GLFWwindow *window,
                               glm::vec2   mousePos,
                               int         button,
                               int         action,
                               int         mods);


  static PlayState* Instance() {
    return &m_PlayState;
  }

protected:

  PlayState() {}

private:

  static PlayState m_PlayState;
  bool renderDebug;
  SharedShaderProgram skyboxShader;
  SharedShaderProgram cubeShader;
  SharedArrayBuffer   debug_ab;
  SharedVertexArrayObject debug_vao;
  SharedShaderProgram     debugShader;
};
#endif // ifndef PLAYSTATE_H
