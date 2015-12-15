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
#include "world/TestObject.hh"
#include "world/Cloud.hh"

using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;
using namespace ACGL::Scene;

PlayState PlayState::m_PlayState;
GenericCamera camera;


Skybox *skybox;
TestObject *cube;
Cloud *cloud;

void PlayState::init(CGame *game) {
  renderDebug = false;

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


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
  skybox = new Skybox(Model("cube.obj", 50.0f), paths);
  cube   =
    new TestObject(Model("cube.obj", 1.0f), vec3(0.0f, 0.0f, -1.0f),
                   vec3(0.0f, 0.0f, 0.0f));

  cloud = new Cloud(400);
  cloud->setPosition(vec3(0.0f, 1.0f, -1.0f));
  debug() << "Geometry loaded" << endl;


  debug() << "Loading shaders stage" << endl;

  // cout<<Settings::the()->getFullShaderPath()<<endl;
  // construct VAO to give shader correct Attribute locations
  SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aTexCoord", GL_FLOAT, 3);
  ab->defineAttribute("aNormal",   GL_FLOAT, 3);

  SharedVertexArrayObject vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);

  cubeShader = ShaderProgramCreator("cube").attributeLocations(
    vao->getAttributeLocations()).create();

  skyboxShader = ShaderProgramCreator("skybox").attributeLocations(
    vao->getAttributeLocations()).create();

  cloudShader = ShaderProgramCreator("Particle").attributeLocations(cloud->getVao()->getAttributeLocations()).create();

  // debug_ab = SharedArrayBuffer(new ArrayBuffer());
  // debug_ab->defineAttribute("aPosition", GL_FLOAT, 3);
  // debug_ab->defineAttribute("aColor", GL_FLOAT, 3);
  // debug_vao = SharedVertexArrayObject(new VertexArrayObject());
  // debug_vao->attachAllAttributes(debug_ab);
  // debug_vao->setMode(GL_LINES);

  // debugShader =
  // ShaderProgramCreator("debug").attributeLocations(debug_vao->getAttributeLocations()).create();


  debug() << "Shaders loaded" << endl;


  debug() << "Set Textures Stage" << endl;
  skyboxShader->use();
  skyboxShader->setTexture("uTexture", skybox->getTexture(), 1);

  cubeShader->use();
  cubeShader->setTexture("uTexture", cube->getTexture(), 2);

  // cloudShader->use();
  // cloudShader->setTexture("", cloud->getTexture(), 3);
  debug() << "Textures set" << endl;


  camera.setVerticalFieldOfView(90.0);
  camera.setPosition(vec3(0.0f, 0.0f, 1.0f));

  // debug()<<"Camera Position: \n"<<to_string(camera.getPosition())<<endl;
  // debug()<<"Camera View: \n"<<to_string(camera.getViewMatrix())<<endl;

  openGLCriticalError();
}

void PlayState::draw(CGame *g, float *delta) {
  // std::cout<<"Draw IntroState at time: "<<*delta<<std::endl;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 viewProjectionMatrix = camera.getProjectionMatrix() *
                                  camera.getViewMatrix();



  cloudShader->use();
  cloud->render(cloudShader, &viewProjectionMatrix);


  cubeShader->use();
  // cubeShader->setUniform( "uNormalMatrix", camera.getRotationMatrix3() );
  cubeShader->setUniform("uViewMatrix", camera.getViewMatrix());
  cube->render(cubeShader, &viewProjectionMatrix);




  glDepthFunc(GL_LEQUAL);
  skyboxShader->use();
  skybox->render(skyboxShader, &viewProjectionMatrix);
  glDepthFunc(GL_LESS);


  // if(renderDebug) {
  //     debugShader->use();
  //     debugShader->setUniform("uViewProjectionMatrix", viewProjectioMatrix);
  //     debug_vao->render();
  // }
  openGLCriticalError();
}

void PlayState::update(CGame *g, float dt) {

  // Sollte glaube ich eher nach render?
  skybox->setPosition(vec3(camera.getPosition().x, 0.0f, camera.getPosition().z));
  cloud->update(dt, 1);
}

void PlayState::handleMouseMoveEvents(GLFWwindow *window, glm::vec2 mousePos) {}

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

  double speed = 5.0;        // magic value to scale the camera speed

  if (action == GLFW_PRESS | action == GLFW_REPEAT) {
    if (key == GLFW_KEY_W) { // upper case!
      camera.moveForward(timeElapsed * speed);
    }

    if (key == GLFW_KEY_Q) { // upper case!
      camera.FPSstyleLookAround(-timeElapsed * speed, 0);
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
    
    }

    if (key == GLFW_KEY_R) {
      ShaderProgramCreator("cube").update(cubeShader);
      ShaderProgramCreator("skybox").update(skyboxShader);
    }
  }
}

void PlayState::handleResizeEvents(GLFWwindow *window, glm::uvec2 windowSize) {
  camera.resize(windowSize.x, windowSize.y);
}
