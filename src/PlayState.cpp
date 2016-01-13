#include "PlayState.hh"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>

#include <ACGL/OpenGL/Data/GeometryDataLoadStore.hh>
#include <ACGL/OpenGL/Data/TextureLoadStore.hh>
#include <ACGL/Scene/GenericCamera.hh>
#include "TextureLoadStore.hh"
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Base/Settings.hh>
#include <ACGL/Math/Math.hh>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "Helper.hh"
#include "Model.hh"
#include "world/Skybox.hh"
#include "world/SkyScraper.hh"
#include "world/Terrain.hh"

using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;
using namespace ACGL::Scene;

PlayState PlayState::m_PlayState;
GenericCamera camera;


Skybox *skybox;
vector<Object*> objects;
Terrain *terrain;


void PlayState::init(CGame *game) {
  renderDebug = false;
  m_game = game;

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  camera.setVerticalFieldOfView(95.0);
  camera.setPosition(vec3(0.0f, 2.0f, 0.0f));
  camera.setTarget(vec3(1.f, .0f, .0f), vec3(.0f, 1.f, .0f));

  // define where shaders and textures can be found:
  Settings::the()->setResourcePath(Helper::getExePath() + "/assets/");
  Settings::the()->setShaderPath("shaders/");
  Settings::the()->setGeometryPath("geometry/");
  Settings::the()->setTexturePath("textures/");


  debug() << "Loading objects stage" << endl;

  // Texture
  std::vector<std::string> paths = {
    Settings::the()->getFullTexturePath() + "nuke_rt.png",
    Settings::the()->getFullTexturePath() + "nuke_lf.png",
    Settings::the()->getFullTexturePath() + "nuke_dn.png",
    Settings::the()->getFullTexturePath() + "nuke_up.png",
    Settings::the()->getFullTexturePath() + "nuke_bk.png",
    Settings::the()->getFullTexturePath() + "nuke_ft.png",
  };
  // skybox = new Skybox(Model("cube.obj", 50.0f), paths);
  Object* skyScraper = new SkyScraper(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f));
  objects.push_back(skyScraper);
  terrain = new Terrain();

  debug() << "Geometry loaded" << endl;


  debug() << "Loading shaders stage" << endl;

  // cout<<Settings::the()->getFullShaderPath()<<endl;
  // construct VAO to give shader correct Attribute locations
  SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aNormal",   GL_FLOAT, 3);

  SharedVertexArrayObject vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);

  skyboxShader = ShaderProgramCreator("skybox").attributeLocations(
  vao->getAttributeLocations()).create();

  lightningShader = ShaderProgramCreator("lightningShader").attributeLocations(
    vao->getAttributeLocations()).create();

  // debug_ab = SharedArrayBuffer(new ArrayBuffer());
  // debug_ab->defineAttribute("aPosition", GL_FLOAT, 3);
  // debug_ab->defineAttribute("aColor", GL_FLOAT, 3);
  // debug_vao = SharedVertexArrayObject(new VertexArrayObject());
  // debug_vao->attachAllAttributes(debug_ab);
  // debug_vao->setMode(GL_LINES);

  debugShader =
  ShaderProgramCreator("debug").attributeLocations(vao->getAttributeLocations()).create();


  debug() << "Shaders loaded" << endl;


  debug() << "Set Textures Stage" << endl;
  // skyboxShader->use();
  // skyboxShader->setTexture("uTexture", skybox->getTexture(), 1);

  // cubeShader->use();
  // cubeShader->setTexture("uTexture", cube->getTexture(), 2);

  //terrainShader->use();
  //terrainShader->setTexture("uTexture", cube->getTexture(), 2);

  // debug() << "Texture for skybox: " << skybox->getTexture() << endl;
  debug() << "Textures set" << endl;


  // debug()<<"Camera Position: \n"<<to_string(camera.getPosition())<<endl;
  // debug()<<"Camera View: \n"<<to_string(camera.getViewMatrix())<<endl;

  openGLCriticalError();
  glfwSetInputMode(game->g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void PlayState::draw(CGame *g, float *delta) {
  // std::cout<<"Draw IntroState at time: "<<*delta<<std::endl;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


  glm::mat4 viewProjectionMatrix = camera.getProjectionMatrix() *
                                  camera.getViewMatrix();

  //glDepthFunc(GL_LEQUAL);
  // skyboxShader->use();
  // skybox->render(skyboxShader, &viewProjectionMatrix);
  glDepthFunc(GL_LESS);

openGLCriticalError();

  lightningShader->use();
  lightningShader->setUniform("uLight.direction", vec3(-1,1.5,1));
  lightningShader->setUniform("uLight.color", vec3(0.75f, 0.75f, 0.75f));
  lightningShader->setUniform("uLight.ambient", 0.1f);
  lightningShader->setUniform("uLight.specular", 0.3f);
  lightningShader->setUniform("uLight.diffuse", 0.8f);
  lightningShader->setUniform("uViewMatrix", camera.getViewMatrix());
  lightningShader->setUniform("camera", camera.getPosition());
  terrain->render(lightningShader, &viewProjectionMatrix);


  for(int i = 0; i<objects.size(); i++) {
    objects[i]->render(lightningShader, &viewProjectionMatrix);  
  }
  

  if(renderDebug) {
      debugShader->use();
      terrain->render(debugShader, &viewProjectionMatrix);
      for(int i = 0; i<objects.size(); i++) {
        objects[i]->render(debugShader, &viewProjectionMatrix);  
      }
  }

  //glFlush();
  // bring backbuffer to foreground
  // SDL_GL_SwapBuffers();
  //SwapBuffers(g_HDC);

  openGLCriticalError();
}

void PlayState::update(CGame *g, float delta) {
//  skybox->setPosition(vec3(camera.getPosition().x, 0.0f, camera.getPosition().z));
  //camera.FPSstyleLookAround(0.001f, 0.0f);
}

void PlayState::handleMouseMoveEvents(GLFWwindow *window, glm::vec2 mousePos) {
  m_lastMousePos = m_mousePos;
  m_mousePos = mousePos;

  //Update FPS Camera for Debug:
  vec2 mouseDelta = (m_lastMousePos - m_mousePos);
  camera.FPSstyleLookAround(mouseDelta.x/m_game->g_windowSize.x, mouseDelta.y/m_game->g_windowSize.y);
}

void PlayState::handleMouseButtonEvents(GLFWwindow *window,
                                        glm::vec2   mousePos,
                                        int         button,
                                        int         action,
                                        int         mods) {}

void PlayState::handleKeyEvents(GLFWwindow *window,
                                int         key,
                                int         scancode,
                                int         action,
                                int         mods) {
  CGameState::handleKeyEvents(window, key, scancode, action, mods);

  // make camera movements based on the elapsed time and not based on frames
  // rendered!
  double timeElapsed = .05;

  double speed = 100.0;        // magic value to scale the camera speed

  if ((action == GLFW_PRESS) | (action == GLFW_REPEAT)) {
    if (key == GLFW_KEY_W) { // upper case!
      camera.moveForward(timeElapsed * speed);
    }

    if (key == GLFW_KEY_S) { // upper case!
      camera.moveBack(timeElapsed * speed);
    }

    if (key == GLFW_KEY_E) { // upper case!
      camera.FPSstyleLookAround(timeElapsed * speed, 0);
    }

    if (key == GLFW_KEY_A) { // upper case!
      camera.moveLeft(timeElapsed * speed);
    }

    if (key == GLFW_KEY_D) { // upper case!
      camera.moveRight(timeElapsed * speed);
    }

    if (key == GLFW_KEY_P) {
      renderDebug = !renderDebug;
      if(renderDebug) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);} else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
    }
    if (key == GLFW_KEY_R) {

      ShaderProgramCreator("lightningShader").update(lightningShader);
    }
  }
}



void PlayState::handleResizeEvents(GLFWwindow *window, glm::uvec2 windowSize) {
  camera.resize(windowSize.x, windowSize.y);
}
