#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.hh"

#ifndef GLINCLUDED
#define GLINCLUDED
# include <ACGL/OpenGL/GL.hh>
# include <GLFW/glfw3.h>
#endif


#include "world/SkyDome.hh"
#include "world/SkyScraper.hh"
#include "world/Terrain.hh"
#include "LoadingScreen.hh"
#include <ACGL/OpenGL/Objects/FrameBufferObject.hh>
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
  bool renderDebug;
  bool showFrames = false;
  SharedShaderProgram skydomeShader;
  SharedShaderProgram lightningShader;
  SharedShaderProgram cloudShader;
  SharedShaderProgram clothShader;
  SharedShaderProgram depthShader;
  SharedVertexArrayObject blendVAO;
  SharedTexture2D zBufferTexture;
  SharedFrameBufferObject zBuffer;
  SharedArrayBuffer   debug_ab;
  SharedVertexArrayObject debug_vao;
  SharedShaderProgram     debugShader;
  Level* level;
  LoadingScreen*  loadingScreen;
  float timeSinceStart = 0.f;
  float lastTime;

  Gui* eventGui;
  PositionGUI* positionGui;
  GUIObject* fpsGraph;
  Text* msg;

};
#endif // ifndef PLAYSTATE_H
