#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.hh"

#include <ACGL/OpenGL/GL.hh>
#include <GLFW/glfw3.h>


#include "world/SkyDome.hh"
#include "world/SkyScraper.hh"
#include "world/Terrain.hh"
#include <ACGL/Scene/GenericCamera.hh>

#include <ACGL/OpenGL/Creator/VertexArrayObjectCreator.hh>

using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;
using namespace ACGL::Scene;


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
  //int m_MouseState;
  glm::vec2   m_mousePos, m_lastMousePos;
  static PlayState m_PlayState;
  bool renderDebug, showFrames;
  SharedShaderProgram skydomeShader;
  SharedShaderProgram lightningShader;
  SharedArrayBuffer   debug_ab;
  SharedVertexArrayObject debug_vao;
  SharedShaderProgram     debugShader;

  SkyDome *skydome;
  vector<Object*> objects;
  Terrain *terrain;
  GUIObject *graph;

  GenericCamera camera;
  Level* level;


};
#endif // ifndef PLAYSTATE_H
