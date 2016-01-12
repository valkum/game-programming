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
#include <glm/gtc/random.hpp>


#include <iostream>
#include <vector>
#include "Helper.hh"
#include "Model.hh"
#include "world/Skybox.hh"
#include "world/Vec3.hh"
#include "world/TestObject.hh"
#include "world/Cloud.hh"
#include "world/Cloth.hh"


//#define TIME_STEPSIZE2 0.25f*0.25f
// 1/64 Tickrate
#define TIME_STEPSIZE2 0.0156f

using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;
using namespace ACGL::Scene;

PlayState PlayState::m_PlayState;
GenericCamera camera;

Skybox *skybox;
//TestObject *cube;
Cloud *cloud;
Cloth *cloth;
float ball_time = 0;
float ball_radius = 2;
vec3 ball_pos(7,-5,0);



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

PerfGraph *graph;

void PlayState::init(CGame *game) {
  renderDebug = false;

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  gui = new Gui(vg, game->g_window);

  graph = new PerfGraph(gui, GRAPH_RENDER_FPS, "FPS meter");
  graph->setPosition(ivec2(400,400));
  graph->setSize(ivec2(200,35));


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

  cloud = new Cloud(500);
  cloud->setPosition(vec3(3.0f, 3.0f, -1.0f));

  // cube   =
  //   new TestObject(Model("cube.obj", 1.0f), vec3(0.0f, 0.0f, -1.0f),
  //                  vec3(0.0f, 0.0f, 0.0f));
  // cube   = new TestObject(Model("aphroditegirl.obj", 100.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f));
  cloth = new Cloth(10,20,24,24);

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

  // cubeShader = ShaderProgramCreator("cube").attributeLocations(
    // vao->getAttributeLocations()).create();
  clothShader = ShaderProgramCreator("cloth").attributeLocations(
    cloth->getVAO()->getAttributeLocations()).create();

  skyboxShader = ShaderProgramCreator("skybox").attributeLocations(
    vao->getAttributeLocations()).create();

  cloudShader = ShaderProgramCreator("cloudParticle").attributeLocations(cloud->getVao()->getAttributeLocations()).create();

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
  // skyboxShader->use();
  // skyboxShader->setTexture("uTexture", skybox->getTexture(), 1);


  // cubeShader->use();
  // cubeShader->setTexture("uTexture", cube->getTexture(), 2);

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
  skybox->setPosition(vec3(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 viewMatrix = camera.getViewMatrix();
  glm::mat4 viewProjectionMatrix = camera.getProjectionMatrix() *
                                  viewMatrix;

  glm::vec3 cameraRight_worldspace = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
  glm::vec3 cameraUp_worldspace = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

  // glDepthFunc(GL_LEQUAL);
  // skyboxShader->use();
  // skybox->render(skyboxShader, &viewProjectionMatrix);
  // glDepthFunc(GL_LESS);




  // cubeShader->use();
  // // cubeShader->setUniform( "uNormalMatrix", camera.getRotationMatrix3() );
  // cubeShader->setUniform("uViewMatrix", camera.getViewMatrix());
  // cube->render(cubeShader, &viewProjectionMatrix);

  cloudShader->use();
  cloudShader->setUniform("uCameraRight_worldspace", cameraRight_worldspace);
  cloudShader->setUniform("uCameraUp_worldspace", cameraUp_worldspace);
  cloud->render(cloudShader, &viewProjectionMatrix);




  // drawing

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  clothShader->use();
  cloth->render(clothShader, &viewProjectionMatrix); // finally draw the cloth with smooth shading

  // if(renderDebug) {
  //     debugShader->use();
  //     debugShader->setUniform("uViewProjectionMatrix", viewProjectionMatrix);
  //     debug_vao->render();
  // }
  openGLCriticalError();

  gui->drawAll();
}

void PlayState::update(CGame *g, float dt) {
  cloud->update(dt);

  cloth->addForce(vec3(0.0f,-9.0f,0.0f)*dt); // add gravity each frame, pointing down
  glm::vec3 random = sphericalRand(0.5f);
  cloth->windForce((vec3(0.3f,0.3f,0.03f)+random)*dt); // generate some wind each frame
  cloth->timeStep(dt); // calculate the particle positions of the next frame

  // Sollte glaube ich eher nach render?
  cloud->update(dt);
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
    if (key == GLFW_KEY_Q) { // upper case!
      camera.FPSstyleLookAround(-timeElapsed * speed, 0); // turn left
    }

    if (key == GLFW_KEY_W) { // upper case!
      camera.moveForward(timeElapsed * speed);
    }

    if (key == GLFW_KEY_E) { // upper case!
      camera.FPSstyleLookAround(timeElapsed * speed, 0); // turn right
    }

    if (key == GLFW_KEY_I) { // upper case!
      camera.FPSstyleLookAround(0, -timeElapsed * speed); // turn upwards
    }

    if (key == GLFW_KEY_A) { // upper case!
      camera.moveLeft(timeElapsed * speed);
    }

    if (key == GLFW_KEY_S) { // upper case!
      camera.moveBack(timeElapsed * speed);
    }

    if (key == GLFW_KEY_D) { // upper case!
      camera.moveRight(timeElapsed * speed);
    }

    if (key == GLFW_KEY_J) { // upper case!
      camera.FPSstyleLookAround(-timeElapsed * speed, 0); // turn left
    }

    if (key == GLFW_KEY_K) { // upper case!
      camera.FPSstyleLookAround(0, timeElapsed * speed); // turn downwards
    }

    if (key == GLFW_KEY_L) { // upper case!
      camera.FPSstyleLookAround(timeElapsed * speed, 0); // turn right
    }

    if (key == GLFW_KEY_P) {
<<<<<<< Updated upstream
      for (int i = 0; i < 10; ++i)
      {
      debug() << graph->values[i] << endl;
      }
=======
      cloud->spawn(5);
>>>>>>> Stashed changes
    }

    if (key == GLFW_KEY_R) {
      ShaderProgramCreator("cube").update(cubeShader);
      ShaderProgramCreator("skybox").update(skyboxShader);
      ShaderProgramCreator("cloth").update(skyboxShader);
    }
    if (key == GLFW_KEY_SPACE) {
      cloth->windForce((vec3(1.0f,-0.2f,0.3f)));
    }
  }
}

void PlayState::handleResizeEvents(GLFWwindow *window, glm::uvec2 windowSize) {
  camera.resize(windowSize.x, windowSize.y);
}
