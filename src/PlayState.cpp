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
#include "world/Vec3.hh"
#include "world/TestObject.hh"
#include "world/Cloth.hh"


#define TIME_STEPSIZE2 0.25*0.25

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
Cloth *cloth;
float ball_time = 0;
float ball_radius = 2;
Vec3 ball_pos(7,-5,0);



/*
void reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();  
	if (h==0)  
		gluPerspective(80,(float)w,1.0,5000.0);
	else
		gluPerspective (80,( float )w /( float )h,1.0,5000.0 );
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity(); 
}
*/


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
  debug() << "Geometry loaded" << endl;

  cloth = new Cloth(13,10,55,45);
  Vec3 ball_pos(7,-5,0); // the center of our one ball

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

  debug() << "Texture for cube: " << skybox->getTexture() << endl;
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

  glm::mat4 viewProjectioMatrix = camera.getProjectionMatrix() *
                                  camera.getViewMatrix();


  cubeShader->use();

  // cubeShader->setUniform( "uNormalMatrix", camera.getRotationMatrix3() );
  cubeShader->setUniform("uViewMatrix", camera.getViewMatrix());
  cube->render(cubeShader, &viewProjectioMatrix);

  glDepthFunc(GL_LEQUAL);
  skyboxShader->use();
  skybox->render(skyboxShader, &viewProjectioMatrix);
  glDepthFunc(GL_LESS);

  // calculating positions
  ball_time++;
  ball_pos.f[2] = cos(ball_time/50.0)*7;

  cloth->addForce(Vec3(0.0f,-0.2f,0.0f)*TIME_STEPSIZE2); // add gravity each frame, pointing down
  cloth->windForce(Vec3(0.5,0,0.2)*TIME_STEPSIZE2); // generate some wind each frame
  cloth->timeStep(); // calculate the particle positions of the next frame
  cloth->ballCollision(ball_pos,ball_radius); // resolve collision with the ball

  // drawing

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glLoadIdentity();

  cloth->drawShaded(); // finally draw the cloth with smooth shading

  // if(renderDebug) {
  //     debugShader->use();
  //     debugShader->setUniform("uViewProjectionMatrix", viewProjectioMatrix);
  //     debug_vao->render();
  // }
  openGLCriticalError();
}

void PlayState::update(CGame *g, float delta) {
  skybox->setPosition(vec3(camera.getPosition().x, 0.0f, camera.getPosition().z));
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

  if ((action == GLFW_PRESS) | (action == GLFW_REPEAT)) {
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
      debug() << "uViewProjectionMatrix Location = " << glGetUniformLocation(
        cubeShader->getObjectName(),
        "uViewProjectionMatrix") << endl;
      debug() << "uViewMatrix Location = " << glGetUniformLocation(
        cubeShader->getObjectName(),
        "uViewMatrix") << endl;
      debug() << "uMVP Location = " << glGetUniformLocation(
        cubeShader->getObjectName(),
        "uMVP") << endl;
      debug() << "uProjectionMatrix Location = " << glGetUniformLocation(
        cubeShader->getObjectName(),
        "uProjectionMatrix") << endl;
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
