#include "PlayState.hh"
#include "IntroState.hh"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>

#include <ACGL/OpenGL/Data/GeometryDataLoadStore.hh>
#include <ACGL/OpenGL/Data/TextureLoadStore.hh>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Math/Math.hh>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <vector>
#include "easing.hh"
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
bool freeCamera = false;

float testRotationAngle = 0;
float cameraPos = 0;

float speedBuildUp = 0.0f;
float fadeOutOpacity = 0.001f;

bool aPressed = false;
bool dPressed = false;
bool wPressed = false;

bool win = false;
bool collision = false;

PositionGUI* positionGui;

void PlayState::init(CGame *game) {
  loadingScreen = new LoadingScreen();
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
  fpsGraph->setPosition(ivec2(10,60));
  fpsGraph->setSize(ivec2(200,35));
  loadingScreen->render(0.3);

  level = new Level(game->cli_settings.levelId);

  //Level* level = new Level("00000-00001");
  level->load();
  loadingScreen->render(0.5);

  positionGui = new PositionGUI(gui, "Position");
  positionGui->setPosition(ivec2(10, 20));
    
  character = new Character(vec3(0.0f, 2.0f, 5.0f), vec3(0.0f, M_PI, 0.0f), 0.02f);


  float quadVertices[] = {
    -1.f, -1.f, 0.f,
    -1.f, 1.f, 0.f,
    1.f, -1.f, 0.f,
    1.f, 1.f, 0.f
  };

  ACGL::OpenGL::SharedArrayBuffer fullQuadAB = ACGL::OpenGL::SharedArrayBuffer(new ACGL::OpenGL::ArrayBuffer());
  fullQuadAB->defineAttribute("aPosition", GL_FLOAT, 3);
  fullQuadAB->setDataElements(4, quadVertices);

  blendVAO = ACGL::OpenGL::SharedVertexArrayObject(new ACGL::OpenGL::VertexArrayObject());
  blendVAO->attachAllAttributes(fullQuadAB);
  blendVAO->setMode(GL_TRIANGLE_STRIP);

  loadingScreen->render(0.7);


  // construct VAO to give shader correct Attribute locations
  SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aNormal",   GL_FLOAT, 3);

  SharedVertexArrayObject vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);

  cloudShader = ShaderProgramCreator("cloudParticle").attributeLocations(level->getClouds()->getVao()->getAttributeLocations()).create();

  skydomeShader = ShaderProgramCreator("skyDome").attributeLocations(
    level->getSkydome()->getModel().getVAO()->getAttributeLocations()).create();

  lightningShader = ShaderProgramCreator("lightningShader").attributeLocations(
    vao->getAttributeLocations()).create();

  debugShader =
  ShaderProgramCreator("debug").attributeLocations(vao->getAttributeLocations()).create();
  loadingScreen->render(0.8);

  skydomeShader->use();
  skydomeShader->setTexture("uTexture", level->getSkydome()->getTexture(), 2);

  loadingScreen->render(0.9);
  openGLCriticalError();
  glfwSetInputMode(game->g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  loadingScreen->render(1);
  lastTime = glfwGetTime();
  timeSinceStart = 0;
}

void PlayState::draw(CGame *g, float *delta) {
  timeSinceStart += glfwGetTime() - lastTime;
  lastTime = glfwGetTime();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  // @TODO: render white fade in if glfwGetTme() < levelStartTime + 1

  if(renderDebug) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  ACGL::Scene::GenericCamera* camera = level->getCamera();
  glm::mat4 viewMatrix = camera->getViewMatrix();
  glm::mat4 projectionMatrix = camera->getProjectionMatrix();
  glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

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
  lightningShader->setUniform("uLight.direction", level->getLight().direction); //vec3(-1,1.5,1));
  lightningShader->setUniform("uLight.color", level->getLight().color); //vec3(0.75f, 0.75f, 0.75f));
  lightningShader->setUniform("uLight.ambient", level->getLight().ambient); //0.05f);
  lightningShader->setUniform("uLight.specular", level->getLight().specular); //0.2f);
  lightningShader->setUniform("uLight.diffuse", level->getLight().diffuse); //0.3f);
  lightningShader->setUniform("uViewMatrix", camera->getViewMatrix());
  lightningShader->setUniform("camera", camera->getPosition());
  lightningShader->setUniform("uColor", vec3(0.75f, 0.75f, 0.75f));
  openGLCriticalError();
  level->getTerrain()->render(lightningShader, &viewProjectionMatrix);
  openGLCriticalError();
  
  for (auto object: level->getObjects()) {
    object->render(lightningShader, &viewProjectionMatrix);
  }
  character->render(lightningShader, &viewProjectionMatrix);

  cloudShader->use();
  cloudShader->setUniform("uTime", *delta);
  cloudShader->setUniform("uViewMatrix", (viewMatrix));
  cloudShader->setUniform("uProjectionMatrix", (projectionMatrix));
  cloudShader->setUniform("uViewProjectionMatrix", (projectionMatrix) * (viewMatrix));
  // cloudShader->setUniform("uCameraRight_worldspace", vec3(camera->getViewMatrix()[0][0], camera->getViewMatrix()[1][0], camera->getViewMatrix()[2][0]));
  // cloudShader->setUniform("uCameraUp_worldspace", vec3(camera->getViewMatrix()[0][1], camera->getViewMatrix()[1][1], camera->getViewMatrix()[2][1]));
  level->getClouds()->render(cloudShader, &viewMatrix, &projectionMatrix);
  //level->getClouds()->render(cloudShader, &viewProjectionMatrix, camera->getPosition());

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
  


  if(timeSinceStart <= 3.0f) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    SharedShaderProgram loadingShader= loadingScreen->getShader();
    loadingShader->use();
    float opacity = 1 - quarticInOut(timeSinceStart/3); //3sec opacity from 1 to 0
    loadingShader->setUniform("uColor", vec4(0.99f,.99f,.99f, opacity));
    blendVAO->render();

    speedBuildUp = 0.1f;
  }
  if((win || collision) && level->getCamera()->getPosition().y >= 15.0f){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    SharedShaderProgram loadingShader= loadingScreen->getShader();
    loadingShader->use();
    fadeOutOpacity *= 1.1f;
    if (fadeOutOpacity >= 1.0f) {
      fadeOutOpacity = 1.0f;
      // Hacky methode um wieder zum Menu zu kommen.
      win = false;
      collision = false;

      CGame *g = CGame::instance();
      g->changeState(IntroState::instance());
    }
    loadingShader->setUniform("uColor", vec4(0.99f,.99f,.99f, fadeOutOpacity));
    blendVAO->render();
  }

  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}



void PlayState::handleMouseMoveEvents(GLFWwindow *window, glm::vec2 mousePos) {
  if(freeCamera){
    m_lastMousePos = m_mousePos;
    m_mousePos = mousePos;

    //Update FPS Camera for Debug:
    vec2 mouseDelta = (m_lastMousePos - m_mousePos);
    level->getCamera()->FPSstyleLookAround(-mouseDelta.x/m_game->g_windowSize.x, -mouseDelta.y/m_game->g_windowSize.y);
    positionGui->setCameraDirection(level->getCamera()->getForwardDirection());
  }
}


void PlayState::update(CGame *g, float dt) {
  if (!collision && !win){
    if(level->collisionDetection(character->getPosition(), vec3(0.0f, 0.0f, 0.0f), character->getScale())){
      collision = true;
    }

    vec3 charPos = vec3(0.0f, 0.0f, 0.0f);

    if (speedBuildUp < 1){
      speedBuildUp *= 1.01;
    }

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
    else if(aPressed) {
      if(cameraPos > -1.0f){
        cameraPos -= 0.025f;
      }
      charPos += vec3(0.05f, 0.0f, 0.0f);
    }else if(dPressed) {
      if(cameraPos < 1.0f){
        cameraPos += 0.025f;
      }
      charPos -= vec3(0.05f, 0.0f, 0.0f);
    }

    //testRotationAngle += 5.0f;
    float radian = cameraPos * 45 * M_PI / 180;
    character->rotateZ(-radian);

    charPos += speedBuildUp * vec3(0.0f, 0.0f, 0.4f);
    if(!collision){
      charPos += vec3(0.0f, 0.0f, 0.005f);
    }else{
      charPos = vec3(0.0f, 0.0f, 0.0f);
    }
    character->update(dt);
    character->setCharacterPosition(character->getPosition() + charPos);

    if(character->getPosition().z >= 980){
      win = true;
    }
    if(!freeCamera){
      level->getCamera()->setPosition(character->getPosition() + vec3(-0.1f*cameraPos, 0.16f, -0.40f));
    }
    if(wPressed){
      level->getCamera()->moveForward(0.1f);
    }
  }else{
    if(level->getCamera()->getPosition().y < 30 && !freeCamera){
      level->getCamera()->setPosition(level->getCamera()->getPosition() + vec3(0.0f, 0.05f, 0.0f));

    }else if(wPressed && freeCamera){
      level->getCamera()->moveForward(0.1f);
    }
  }
  level->getClouds()->update(dt, level->getCamera()->getPosition(), level->getCamera()->getProjectionMatrix() * level->getCamera()->getViewMatrix(), level->getWind() * 0.05f);
  positionGui->setCameraPosition(level->getCamera()->getPosition());
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
      wPressed = true;
      //debug()<<"Key W, pressed, or repeated"<<std::endl;
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
      //debug()<<"Key A, pressed, or repeated"<<std::endl;
    }

    if (key == GLFW_KEY_D) { // upper case!
      //level->getCamera()->moveRight(timeElapsed * speed);
      //positionGui->setCameraPosition(level->getCamera()->getPosition());
      character->setCharacterPosition(character->getPosition() + vec3(-0.05f, 0.0f, 0.0f));
      dPressed = true;
      //debug()<<"Key D, pressed, or repeated"<<std::endl;
    }
    if (key == GLFW_KEY_F) {
      showFrames = !showFrames;
      if(showFrames) positionGui->show(); else positionGui->hide(); 
    }

    if (key == GLFW_KEY_P) {
      renderDebug = !renderDebug;
    }
    if (key == GLFW_KEY_M) {

      win = false;
      collision = false;
      CGame *g = CGame::instance();
      g->changeState(IntroState::instance());
    }
    if (key == GLFW_KEY_R) {
      level->reloadLevel();
      level->loadLight();
    }
    if (key == GLFW_KEY_O) {
      freeCamera = !freeCamera;
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
      aPressed = false;
    }
    if (key == GLFW_KEY_D) {
      dPressed = false;
    }
    if (key == GLFW_KEY_W) {
      wPressed = false;
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
