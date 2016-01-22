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
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <vector>
#include "Helper.hh"
#include "Model.hh"
#include "Sphere.hh"
#include "world/Skybox.hh"
#include "world/Vec3.hh"
#include "world/TestObject.hh"
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
TestObject *lowPolyMan;
Cloth *cloth;
Sphere *sphere0;
Sphere *sphere1;
Sphere *sphere2;


vec3 clothOffset = vec3(0.0f, 0.0f, -3.0f);
bool triggerWind = false;
bool triggerMesh = true;

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
    // cube   =
    //   new TestObject(Model("cube.obj", 1.0f), vec3(0.0f, 0.0f, -1.0f),
    //                  vec3(0.0f, 0.0f, 0.0f));
    lowPolyMan = new TestObject(Model("low_poly_man.obj", 1.0f), vec3(0.0f, -3.5f, -7.0f), vec3(0.0f, 0.0f, 0.0f));


    cloth = new Cloth(10,20,24,24, lowPolyMan->getPosition(), clothOffset);

    //vec3 spherePos = lowPolyMan->getPosition() + clothOffset + cloth->getSphereOffset0() ;
    vec3 spherePos0 = cloth->getSphereOffset0() ;
    vec3 spherePos1 = cloth->getSphereOffset1() ;
    vec3 spherePos2 = cloth->getSphereOffset2() ;

    sphere0 = new Sphere(lowPolyMan->getPosition(), spherePos0);
    sphere1 = new Sphere(lowPolyMan->getPosition(), spherePos1);
    sphere2 = new Sphere(lowPolyMan->getPosition(), spherePos2);

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



    lowPolyManShader = ShaderProgramCreator("low_poly_man").attributeLocations(
            lowPolyMan->getVAO()->getAttributeLocations()).create();

    clothShader = ShaderProgramCreator("cloth").attributeLocations(
            cloth->getVAO()->getAttributeLocations()).create();

    sphereShader0 = ShaderProgramCreator("cloth").attributeLocations(
            sphere0->getVAO()->getAttributeLocations()).create();

    sphereShader1 = ShaderProgramCreator("cloth").attributeLocations(
            sphere1->getVAO()->getAttributeLocations()).create();

    sphereShader2 = ShaderProgramCreator("cloth").attributeLocations(
            sphere2->getVAO()->getAttributeLocations()).create();

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

    //cubeShader->use();
    //cubeShader->setTexture("uTexture", lowPolyMan->getTexture(), 2);

    debug() << "Texture for cube: " << skybox->getTexture() << endl;
    debug() << "Textures set" << endl;


    camera.setVerticalFieldOfView(70.0);
    //camera.setPosition(vec3(-9.3f, 0.0f, 15.0f));
    //camera.setPosition(vec3(0.0f, 0.0f, -12.0f));
    camera.setPosition(vec3(0.0f, 0.0f, 10.0f));
    //camera.setTarget(lowPolyMan->getPosition() + vec3(0.0f, 3.0f, 6.0f), vec3(0.0f, 1.0f, 0.0f));
    //camera.setTarget(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f, 1.0f, 0.0f));

    // debug()<<"Camera Position: \n"<<to_string(camera.getPosition())<<endl;
    // debug()<<"Camera View: \n"<<to_string(camera.getViewMatrix())<<endl;

    openGLCriticalError();
}

void PlayState::draw(CGame *g, float *delta) {
    skybox->setPosition(vec3(camera.getPosition().x, 0.0f, camera.getPosition().z));
    // std::cout<<"Draw IntroState at time: "<<*delta<<std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glm::mat4 viewProjectioMatrix = camera.getProjectionMatrix() *
        camera.getViewMatrix();

    glDepthFunc(GL_LEQUAL);
    skyboxShader->use();
    skybox->render(skyboxShader, &viewProjectioMatrix);
    glDepthFunc(GL_LESS);


    // drawing
    lowPolyManShader->use();
    lowPolyManShader->setUniform("uViewMatrix", camera.getViewMatrix());

    if (triggerMesh) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      lowPolyMan->render(lowPolyManShader, &viewProjectioMatrix);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        lowPolyMan->render(lowPolyManShader, &viewProjectioMatrix);
    }


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    sphereShader0->use();
    sphere0->render(sphereShader0, &viewProjectioMatrix);
    sphereShader1->use();
    sphere1->render(sphereShader1, &viewProjectioMatrix);
    sphereShader2->use();
    sphere2->render(sphereShader2, &viewProjectioMatrix);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    clothShader->use();
    cloth->render(clothShader, &viewProjectioMatrix); // finally draw the cloth with smooth shading

    // if(renderDebug) {
    //     debugShader->use();
    //     debugShader->setUniform("uViewProjectionMatrix", viewProjectioMatrix);
    //     debug_vao->render();
    // }
    openGLCriticalError();

    gui->drawAll();
    glEnable(GL_DEPTH_TEST);
}

void PlayState::update(CGame *g, float dt) {
    cloth->addForce(vec3(0.0f,-9.0f,0.0f)*dt); // add gravity each frame, pointing down
    //glm::vec3 random = sphericalRand(0.5f);
    if (triggerWind) {
        //cloth->windForce((vec3(0.0f, 0.3f,-1.0f)+random)*dt); // generate some wind each frame
        cloth->windForce(vec3(0.0f, 0.05f,-0.2f));
    }
    cloth->timeStep(dt); // calculate the particle positions of the next frame
    //cloth->modelCollision();
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

        if (key == GLFW_KEY_K) { // upper case!
            camera.moveUp(timeElapsed * speed);
        }

        if (key == GLFW_KEY_J) { // upper case!
            camera.moveDown(timeElapsed * speed);
        }

        if (key == GLFW_KEY_DOWN) { // upper case!
            camera.setPosition(vec3(0.0f, 5.0f, -5.0f));
            camera.setTarget(lowPolyMan->getPosition()+vec3(0.0f, 5.0f, 15.0f), vec3(0.0f, 1.0f, 0.0f));
        }
        if (key == GLFW_KEY_UP) {
            camera.setPosition(vec3(0.0f, 0.0f, 10.0f));
            camera.setTarget(lowPolyMan->getPosition(), vec3(0.0f, 1.0f, 0.0f));
        }
        if (key == GLFW_KEY_RIGHT) {
            camera.setPosition(vec3(6.0f, 5.0f, -5.0f));
            camera.setTarget(cloth->getPosition(), vec3(0.0f, 1.0f, 0.0f));
        }
        if (key == GLFW_KEY_LEFT) {
            camera.setPosition(vec3(-6.0f, 5.0f, -5.0f));
            camera.setTarget(cloth->getPosition(), vec3(0.0f, 1.0f, 0.0f));
        }

        if (key == GLFW_KEY_P) {
            for (int i = 0; i < 10; ++i)
            {
                debug() << graph->values[i] << endl;
            }
        }

        if (key == GLFW_KEY_R) {
            //ShaderProgramCreator("cube").update(cubeShader);
            ShaderProgramCreator("low_poly_man").update(lowPolyManShader);
            ShaderProgramCreator("skybox").update(skyboxShader);
            ShaderProgramCreator("cloth").update(skyboxShader);
        }
        if (key == GLFW_KEY_SPACE) {
            //cloth->windForce((vec3(1.0f,-0.2f,0.3f)));
            //cloth->windForce((vec3(0.0f,0.5f,-1.5f)));
            triggerWind = !triggerWind;
        }
        if (key == GLFW_KEY_M) {
            //cloth->windForce((vec3(1.0f,-0.2f,0.3f)));
            //cloth->windForce((vec3(0.0f,0.5f,-1.5f)));
            triggerMesh = !triggerMesh;
        }
        if (key == GLFW_KEY_KP_7) {
          cloth->setPosition(cloth->getPosition() + vec3(0.5f,0.0f,0.0f));
          debug() << to_string(cloth->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_4) {
          cloth->setPosition(cloth->getPosition() - vec3(0.5f,0.0f,0.0f));
          debug() << to_string(cloth->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_8) {
          cloth->setPosition(cloth->getPosition() + vec3(0.0f,0.5f,0.0f));
          debug() << to_string(cloth->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_5) {
          cloth->setPosition(cloth->getPosition() - vec3(0.0f,0.5f,0.0f));
          debug() << to_string(cloth->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_9) {
          cloth->setPosition(cloth->getPosition() + vec3(0.0f,0.0f,0.5f));
          debug() << to_string(cloth->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_6) {
          cloth->setPosition(cloth->getPosition() - vec3(0.0f,0.0f,0.5f));
          debug() << to_string(cloth->getPosition()) << endl;
        }
    }
}

void PlayState::handleResizeEvents(GLFWwindow *window, glm::uvec2 windowSize) {
    camera.resize(windowSize.x, windowSize.y);
}
