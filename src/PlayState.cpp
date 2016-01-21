#include "PlayState.hh"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>

#include <ACGL/OpenGL/Data/GeometryDataLoadStore.hh>
#include <ACGL/OpenGL/Data/TextureLoadStore.hh>
#include "TextureLoadStore.hh"
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Math/Math.hh>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "Level.hh"
#include "Model.hh"


using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;
using namespace ACGL::Scene;

PlayState PlayState::m_PlayState;

void PlayState::init(CGame *game) {
  renderDebug = false;
  m_game = game;

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //camera.setVerticalFieldOfView(95.0);
  camera.setPosition(vec3(0.0f, 2.0f, 0.0f));
  camera.setStateFromString("ACGL_GenericCamera | 1 | (-0.152289,1.16336,2.27811) | ((-0.999868,-0.00162319,-0.0161718),(0,0.995,-0.0998699),(0.0162531,-0.0998567,-0.994869)) | PERSPECTIVE_PROJECTION | MONO | EYE_LEFT | 75 | 1.33333 | 0.064 | 0.1 | 5000 | 500 | (0,0)");

  gui = new Gui(vg, game->g_window);
  graph = new PerfGraph(gui, GRAPH_RENDER_FPS, "FPS meter");
  graph->setPosition(ivec2(400,400));
  graph->setSize(ivec2(200,35));


  if(game->cli_settings.flagLevel) {
    debug() << "klappt" <<std::endl;
  }
  Level* level = new Level("fffff-00000");
  //Level* level = new Level("00000-00001");
  level->load();

  debug() << "Loading objects stage" << endl;

  skydome = new SkyDome(Model("SkyDome.obj", 100.0f), "Sky.png");
  Object* skyscraper = new SkyScraper(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f));
  objects.push_back(skyscraper);
  Object* cube = new Object(Model("cube.obj", 5.0f), vec3(5.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f));
  objects.push_back(cube);

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

  skydomeShader = ShaderProgramCreator("skyDome").attributeLocations(
  skydome->getModel().getVAO()->getAttributeLocations()).create();

  lightningShader = ShaderProgramCreator("lightningShader").attributeLocations(
    vao->getAttributeLocations()).create();

  debugShader =
  ShaderProgramCreator("debug").attributeLocations(vao->getAttributeLocations()).create();
  debug() << "Shaders loaded" << endl;


  debug() << "Set Textures Stage" << endl;
  skydomeShader->use();
  skydomeShader->setTexture("uTexture", skydome->getTexture(), 2);

  // lightningShader->use();
  // lightningShader->setTexture("uTexture", cube->getTexture(), 3);

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
  //skydome->setPosition(vec3(camera.getPosition().x, 0.0f, camera.getPosition().z));

  glDepthFunc(GL_LEQUAL);
  // Workarround needed somhow for nanovg, as nanovg calls glActiveTexture(...). 
  // Found no proper way for use with ACGL if this is nessecary with more than one texture.
  // Saxum works without I think.
  skydome->getTexture()->bind(2);
  skydomeShader->use();
  skydome->render(skydomeShader, &viewProjectionMatrix);
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


  for(uint_t i = 0; i<objects.size(); i++) {
    objects[i]->render(lightningShader, &viewProjectionMatrix);  
  }
  

  if(renderDebug) {
      debugShader->use();
      terrain->render(debugShader, &viewProjectionMatrix);
      for(uint_t i = 0; i<objects.size(); i++) {
        objects[i]->render(debugShader, &viewProjectionMatrix);  
      }
  }

  //glFlush();
  // bring backbuffer to foreground
  // SDL_GL_SwapBuffers();
  //SwapBuffers(g_HDC);

  openGLCriticalError();
  if(showFrames) {
    gui->drawAll();
    glEnable(GL_DEPTH_TEST);
  }

}

void PlayState::update(CGame *g, float delta) {
}

void PlayState::handleMouseMoveEvents(GLFWwindow *window, glm::vec2 mousePos) {
  m_lastMousePos = m_mousePos;
  m_mousePos = mousePos;

  //Update FPS Camera for Debug:
  vec2 mouseDelta = (m_lastMousePos - m_mousePos);
  camera.FPSstyleLookAround(-mouseDelta.x/m_game->g_windowSize.x, -mouseDelta.y/m_game->g_windowSize.y);
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

  double speed = 10.0;        // magic value to scale the camera speed

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
    if (key == GLFW_KEY_F) {
      showFrames = !showFrames;
    }
    if (key == GLFW_KEY_V) {
      debug() << camera.storeStateToString() << std::endl;
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
