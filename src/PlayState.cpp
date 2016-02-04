#include "PlayState.hh"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>

#include <ACGL/OpenGL/Data/GeometryDataLoadStore.hh>
#include <ACGL/OpenGL/Data/TextureLoadStore.hh>
#include "LoadingScreen.hh"
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Math/Math.hh>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <vector>
#include "Model.hh"
#include "world/Cloth.hh"
#include "world/Character.hh"


// 1/64 Tickrate
#define TIME_STEPSIZE2 0.0156f


using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;
using namespace ACGL::Scene;

class PlayState;
PlayState PlayState::m_PlayState;
Character *character;

bool triggerWind = false;
bool triggerMesh = false;

float testRotationAngle = 0;
float cameraPos = 0;

bool aPressed = false;
bool dPressed = false;

PositionGUI* positionGui;


void PlayState::init(CGame *game) {
  LoadingScreen* loadingScreen = new LoadingScreen();
  loadingScreen->render(0.2);
  renderDebug = false;
  m_game = game;
  double* x = new double;
  double* y = new double;
  glfwGetCursorPos(game->g_window, x, y);
  m_mousePos = vec2(*x, *y);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  //camera.setVerticalFieldOfView(95.0);
  // camera.setPosition(vec3(0.0f, 2.0f, 0.0f));
  // camera.setStateFromString("ACGL_GenericCamera | 1 | (-0.152289,1.16336,2.27811) | ((-0.999868,-0.00162319,-0.0161718),(0,0.995,-0.0998699),(0.0162531,-0.0998567,-0.994869)) | PERSPECTIVE_PROJECTION | MONO | EYE_LEFT | 75 | 1.33333 | 0.064 | 0.1 | 5000 | 500 | (0,0)");

  gui = new Gui(vg, game->g_window);
  GUIObject* fpsGraph = new PerfGraph(gui, GRAPH_RENDER_FPS, "FPS meter");
  fpsGraph->setPosition(ivec2(400,400));
  fpsGraph->setSize(ivec2(200,35));
  loadingScreen->render(0.3);

  if(game->cli_settings.flagLevel) {
    level = new Level(game->cli_settings.levelId);
  }else {
    level = new Level("fffff-00000");
  }
  //Level* level = new Level("00000-00001");
  level->load();
  loadingScreen->render(0.5);

  positionGui = new PositionGUI(gui, "Position");
  positionGui->setPosition(ivec2(20, 20));
    
  //character = new Character(vec3(0.0f, 4.0f, 10.0f), vec3(0.0f, 3.2f, 0.0f), 0.5f);
  character = new Character(vec3(0.0f, 2.0f, 5.0f), vec3(0.0f, M_PI, 0.0f), 0.3f);

  // construct VAO to give shader correct Attribute locations
  SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aNormal",   GL_FLOAT, 3);

  SharedVertexArrayObject vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);

  skydomeShader = ShaderProgramCreator("skyDome").attributeLocations(
    level->getSkydome()->getModel().getVAO()->getAttributeLocations()).create();

  lightningShader = ShaderProgramCreator("lightningShader").attributeLocations(
    vao->getAttributeLocations()).create();

  debugShader =
  ShaderProgramCreator("debug").attributeLocations(vao->getAttributeLocations()).create();
  debug() << "Shaders loaded" << endl;

  debug() << "Set Textures Stage" << endl;
  skydomeShader->use();
  skydomeShader->setTexture("uTexture", level->getSkydome()->getTexture(), 2);
  debug() << "Textures set" << endl;
  loadingScreen->render(0.9);
  openGLCriticalError();
  glfwSetInputMode(game->g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  loadingScreen->render(1);

  delete loadingScreen;
}

void PlayState::draw(CGame *g, float *delta) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  // @TODO: render white fade in if glfwGetTme() < levelStartTime + 1

  if(renderDebug) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  ACGL::Scene::GenericCamera* camera = level->getCamera();
  glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() *
                                  camera->getViewMatrix();

  level->getSkydome()->setPosition(vec3(camera->getPosition().x, 0.0f, camera->getPosition().z));

  glDepthFunc(GL_LEQUAL);
  // Workarround needed somhow for nanovg, as nanovg calls glActiveTexture(...).
  // Found no proper way for use with ACGL if this is nessecary with more than one texture.
  // Saxum works without I think.
  level->getSkydome()->getTexture()->bind(2);
  skydomeShader->use();
  level->getSkydome()->render(skydomeShader, &viewProjectionMatrix);
  glDepthFunc(GL_LESS);

  openGLCriticalError();

  lightningShader->use();
  lightningShader->setUniform("uLight.direction", vec3(-1,1.5,1));
  lightningShader->setUniform("uLight.color", vec3(0.75f, 0.75f, 0.75f));
  lightningShader->setUniform("uLight.ambient", 0.1f);
  lightningShader->setUniform("uLight.specular", 0.3f);
  lightningShader->setUniform("uLight.diffuse", 0.8f);
  lightningShader->setUniform("uViewMatrix", camera->getViewMatrix());
  lightningShader->setUniform("camera", camera->getPosition());
  level->getTerrain()->render(lightningShader, &viewProjectionMatrix);

    //sphereShader12 = ShaderProgramCreator("cloth").attributeLocations(
    //        sphere12->getVAO()->getAttributeLocations()).create();

  for (auto object: level->getObjects()) {
    object->render(lightningShader, &viewProjectionMatrix);
  }
  character->render(lightningShader, &viewProjectionMatrix);

  if(renderDebug) {
    debugShader->use();
    level->getTerrain()->render(debugShader, &viewProjectionMatrix);
    for (auto object: level->getObjects()) {
      object->render(debugShader, &viewProjectionMatrix);
    }
    character->render(debugShader, &viewProjectionMatrix);
  }


  openGLCriticalError();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  gui->drawAll();
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


void PlayState::handleMouseMoveEvents(GLFWwindow *window, glm::vec2 mousePos) {
  m_lastMousePos = m_mousePos;
  m_mousePos = mousePos;

  //Update FPS Camera for Debug:
  vec2 mouseDelta = (m_lastMousePos - m_mousePos);
  level->getCamera()->FPSstyleLookAround(-mouseDelta.x/m_game->g_windowSize.x, -mouseDelta.y/m_game->g_windowSize.y);
  positionGui->setCameraDirection(level->getCamera()->getForwardDirection());
}


void PlayState::update(CGame *g, float dt) {
  if((aPressed && dPressed) || !(aPressed || dPressed)) {
    if(cameraPos < 0.f) {
      cameraPos += 0.05f;
    }
    else if(cameraPos > 0.f) {
      cameraPos -= 0.05f;
    }
    if(cameraPos<0.05f && cameraPos>-0.05f){
      cameraPos = 0;
    }
  }
  else if(aPressed && cameraPos > -1.f) {
    cameraPos -= 0.05f;
  }else if(dPressed && cameraPos < 1.f) {
    cameraPos += 0.05f;
  }

  //testRotationAngle += 5.0f;
  float radian = cameraPos * 45 * M_PI / 180;
  character->rotateZ(-radian);

  character->update(dt);
  character->setCharacterPosition(character->getPosition() + vec3(0.0f, 0.0f, 0.01f));
  level->getCamera()->setPosition(character->getPosition() + vec3(-1.5f*cameraPos, 0.5f, -5.0f));
}

void PlayState::handleKeyEvents(GLFWwindow *window,
                                int         key,
                                int         scancode,
                                int         action,
                                int         mods) {
  CGameState::handleKeyEvents(window, key, scancode, action, mods);

  // make camera movements based on the elapsed time and not based on frames
  // rendered!
  double timeElapsed = .05;

  double speed = 10.0;        // magic value to scale the camera speed

  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_W) { // upper case!
      level->getCamera()->moveForward(timeElapsed * speed);
      positionGui->setCameraPosition(level->getCamera()->getPosition());
    }

    if (key == GLFW_KEY_S) { // upper case!
      level->getCamera()->moveBack(timeElapsed * speed);
      positionGui->setCameraPosition(level->getCamera()->getPosition());
    }

    if (key == GLFW_KEY_A) { // upper case!
      //level->getCamera()->moveLeft(timeElapsed * speed);
      //positionGui->setCameraPosition(level->getCamera()->getPosition());
      character->setCharacterPosition(character->getPosition() + vec3(0.05f, 0.0f, 0.0f));
      aPressed = true;
      debug()<<"Key A, pressed, or repeated"<<std::endl;
    }

    if (key == GLFW_KEY_D) { // upper case!
      //level->getCamera()->moveRight(timeElapsed * speed);
      //positionGui->setCameraPosition(level->getCamera()->getPosition());
      character->setCharacterPosition(character->getPosition() + vec3(-0.05f, 0.0f, 0.0f));
      dPressed = true;
      debug()<<"Key D, pressed, or repeated"<<std::endl;
    }
    if (key == GLFW_KEY_F) {
      showFrames = !showFrames;
      if(showFrames) positionGui->show(); else positionGui->hide(); 
    }

    if (key == GLFW_KEY_P) {
      renderDebug = !renderDebug;
    }
    if (key == GLFW_KEY_UP) {
      //testRoatation = testRoatation + vec3(0.0f, 0.0f, 0.2f);
      testRotationAngle += 5.0f;
      float radian = testRotationAngle * M_PI / 180;
      character->rotateZ(radian);
    }
    if (key == GLFW_KEY_DOWN) {
      //testRoatation = testRoatation + vec3(0.0f, 0.0f, -0.2f);
      testRotationAngle -= 5.0f;
      float radian = testRotationAngle * M_PI / 180;
      character->rotateZ(radian);
    }
  }else if(action == GLFW_RELEASE) {
    if (key == GLFW_KEY_A) { 
      debug()<<"Key A, released"<<std::endl;
      aPressed = false;
    }
    if (key == GLFW_KEY_D) {
      dPressed = false;
      debug()<<"Key D, released"<<std::endl;
    }
  }
}



void PlayState::handleResizeEvents(GLFWwindow *window, glm::uvec2 windowSize) {
  level->getCamera()->resize(windowSize.x, windowSize.y);
}

void PlayState::handleMouseButtonEvents(GLFWwindow *window,
                                        glm::vec2   mousePos,
                                        int         button,
                                        int         action,
                                        int         mods) {}
