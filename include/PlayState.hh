#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.hh"

#ifndef GLINCLUDED
#define GLINCLUDED
# include <ACGL/OpenGL/GL.hh>
# include <GLFW/glfw3.h>
#endif

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
  //SharedShaderProgram cubeShader;
  //SharedShaderProgram lowPolyManShader;
  //SharedShaderProgram clothShader;
  SharedShaderProgram characterShader;
  //SharedShaderProgram sphereShader0;
  //SharedShaderProgram sphereShader1;
  //SharedShaderProgram sphereShader2;
  //SharedShaderProgram sphereShader3;
  //SharedShaderProgram sphereShader4;
  //SharedShaderProgram sphereShader5;
  //SharedShaderProgram sphereShader6;
  //SharedShaderProgram sphereShader7;
  //SharedShaderProgram sphereShader8;
  //SharedShaderProgram sphereShader9;
  //SharedShaderProgram sphereShader10;
  //SharedShaderProgram sphereShader11;
  //SharedShaderProgram sphereShader12;
  //SharedShaderProgram sphereShader13;
  //SharedShaderProgram sphereShader14;
  //SharedShaderProgram sphereShader15;
  //SharedShaderProgram sphereShader16;
  //SharedShaderProgram sphereShader17;
  //SharedShaderProgram sphereShader18;
  SharedArrayBuffer   debug_ab;
  SharedVertexArrayObject debug_vao;
  SharedShaderProgram     debugShader;
};
#endif // ifndef PLAYSTATE_H
