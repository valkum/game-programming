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
Sphere *sphere3;
Sphere *sphere4;
Sphere *sphere5;
Sphere *sphere6;
Sphere *sphere7;
Sphere *sphere8;
Sphere *sphere9;
Sphere *sphere10;
Sphere *sphere11;
Sphere *sphere12;
Sphere *sphere13;
Sphere *sphere14;
Sphere *sphere15;
Sphere *sphere16;
Sphere *sphere17;
Sphere *sphere18;


vec3 clothOffset = vec3(4.0f, 4.0f, -3.0f);
bool triggerWind = false;
bool triggerMesh = false;

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


    //cloth = new Cloth(10,20,24,24, lowPolyMan->getPosition(), clothOffset);
    cloth = new Cloth(8,15,24,24, lowPolyMan->getPosition(), clothOffset);

    sphere0  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius0(),  vec3(1.0f, 0.0f, 0.0f), cloth->getSphereOffset0());
    sphere1  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius1(),  vec3(0.0f, 1.0f, 0.0f), cloth->getSphereOffset1());
    sphere2  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius2(),  vec3(0.0f, 1.0f, 0.0f), cloth->getSphereOffset2());
    sphere3  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius3(),  vec3(0.0f, 0.0f, 1.0f), cloth->getSphereOffset3());
    sphere4  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius4(),  vec3(0.0f, 0.0f, 1.0f), cloth->getSphereOffset4());
    sphere5  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius5(),  vec3(1.0f, 0.0f, 0.0f), cloth->getSphereOffset5());
    sphere6  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius6(),  vec3(1.0f, 1.0f, 1.0f), cloth->getSphereOffset6());
    sphere7  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius7(),  vec3(1.0f, 1.0f, 1.0f), cloth->getSphereOffset7());
    sphere8  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius8(),  vec3(1.0f, 0.0f, 0.0f), cloth->getSphereOffset8());
    sphere9  = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius9(),  vec3(1.0f, 0.0f, 0.0f), cloth->getSphereOffset9());
    sphere10 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius10(), vec3(0.0f, 1.0f, 1.0f), cloth->getSphereOffset10());
    sphere11 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius11(), vec3(0.0f, 1.0f, 1.0f), cloth->getSphereOffset11());
    sphere12 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius12(), vec3(0.0f, 1.0f, 1.0f), cloth->getSphereOffset12());
    sphere13 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius13(), vec3(1.0f, 0.0f, 0.0f), cloth->getSphereOffset13());
    sphere14 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius14(), vec3(1.0f, 0.0f, 0.0f), cloth->getSphereOffset14());
    sphere15 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius15(), vec3(0.0f, 0.0f, 1.0f), cloth->getSphereOffset15());
    sphere16 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius16(), vec3(0.0f, 0.0f, 1.0f), cloth->getSphereOffset16());
    sphere17 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius17(), vec3(0.0f, 1.0f, 0.0f), cloth->getSphereOffset17());
    sphere18 = new Sphere(lowPolyMan->getPosition(), cloth->getSphereRadius18(), vec3(0.0f, 1.0f, 0.0f), cloth->getSphereOffset18());

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

    sphereShader3 = ShaderProgramCreator("cloth").attributeLocations(
            sphere3->getVAO()->getAttributeLocations()).create();

    sphereShader4 = ShaderProgramCreator("cloth").attributeLocations(
            sphere4->getVAO()->getAttributeLocations()).create();

    sphereShader5 = ShaderProgramCreator("cloth").attributeLocations(
            sphere5->getVAO()->getAttributeLocations()).create();

    sphereShader6 = ShaderProgramCreator("cloth").attributeLocations(
            sphere6->getVAO()->getAttributeLocations()).create();

    sphereShader7 = ShaderProgramCreator("cloth").attributeLocations(
            sphere7->getVAO()->getAttributeLocations()).create();

    sphereShader8 = ShaderProgramCreator("cloth").attributeLocations(
            sphere8->getVAO()->getAttributeLocations()).create();

    sphereShader9 = ShaderProgramCreator("cloth").attributeLocations(
            sphere9->getVAO()->getAttributeLocations()).create();

    sphereShader10 = ShaderProgramCreator("cloth").attributeLocations(
            sphere10->getVAO()->getAttributeLocations()).create();

    sphereShader11 = ShaderProgramCreator("cloth").attributeLocations(
            sphere11->getVAO()->getAttributeLocations()).create();

    sphereShader12 = ShaderProgramCreator("cloth").attributeLocations(
            sphere12->getVAO()->getAttributeLocations()).create();

    sphereShader13 = ShaderProgramCreator("cloth").attributeLocations(
            sphere13->getVAO()->getAttributeLocations()).create();

    sphereShader14 = ShaderProgramCreator("cloth").attributeLocations(
            sphere14->getVAO()->getAttributeLocations()).create();

    sphereShader15 = ShaderProgramCreator("cloth").attributeLocations(
            sphere15->getVAO()->getAttributeLocations()).create();

    sphereShader16 = ShaderProgramCreator("cloth").attributeLocations(
            sphere16->getVAO()->getAttributeLocations()).create();

    sphereShader17 = ShaderProgramCreator("cloth").attributeLocations(
            sphere17->getVAO()->getAttributeLocations()).create();

    sphereShader18 = ShaderProgramCreator("cloth").attributeLocations(
            sphere18->getVAO()->getAttributeLocations()).create();

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
    camera.setPosition(vec3(0.0f, 0.0f, 10.0f));
    //camera.setPosition(vec3(10.0f, -5.0f, -6.0f));
    //camera.setTarget(lowPolyMan->getPosition() + vec3(0.0f, 3.0f, 6.0f), vec3(0.0f, 1.0f, 0.0f));
    //camera.setTarget(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

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
    sphereShader3->use();
    sphere3->render(sphereShader3, &viewProjectioMatrix);
    sphereShader4->use();
    sphere4->render(sphereShader4, &viewProjectioMatrix);
    sphereShader5->use();
    sphere5->render(sphereShader5, &viewProjectioMatrix);
    sphereShader6->use();
    sphere6->render(sphereShader6, &viewProjectioMatrix);
    sphereShader7->use();
    sphere7->render(sphereShader7, &viewProjectioMatrix);
    sphereShader8->use();
    sphere8->render(sphereShader8, &viewProjectioMatrix);
    sphereShader9->use();
    sphere9->render(sphereShader9, &viewProjectioMatrix);
    sphereShader10->use();
    sphere10->render(sphereShader10, &viewProjectioMatrix);
    sphereShader11->use();
    sphere11->render(sphereShader11, &viewProjectioMatrix);
    sphereShader12->use();
    sphere12->render(sphereShader12, &viewProjectioMatrix);
    sphereShader13->use();
    sphere13->render(sphereShader13, &viewProjectioMatrix);
    sphereShader14->use();
    sphere14->render(sphereShader14, &viewProjectioMatrix);
    sphereShader15->use();
    sphere15->render(sphereShader15, &viewProjectioMatrix);
    sphereShader16->use();
    sphere16->render(sphereShader16, &viewProjectioMatrix);
    sphereShader17->use();
    sphere17->render(sphereShader17, &viewProjectioMatrix);
    sphereShader18->use();
    sphere18->render(sphereShader18, &viewProjectioMatrix);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    clothShader->use();
    cloth->render(clothShader, &viewProjectioMatrix); // finally draw the cloth with smooth shading

    // if(renderDebug) {
    //     debugShader->use();
    //     debugShader->setUniform("uViewProjectionMatrix", viewProjectioMatrix);
    //     debug_vao->render();
    // }
    openGLCriticalError();

    //gui->drawAll();
    glEnable(GL_DEPTH_TEST);
}

void PlayState::update(CGame *g, float dt) {
    cloth->addForce(vec3(0.0f,-9.0f,0.0f)*dt); // add gravity each frame, pointing down
    //glm::vec3 random = sphericalRand(0.5f);
    if (triggerWind) {
        //cloth->windForce((vec3(0.0f, 0.3f,-1.0f)+random)*dt); // generate some wind each frame
        cloth->windForce(vec3(0.0f, 0.05f,0.2f));
    }
    cloth->timeStep(dt); // calculate the particle positions of the next frame
    cloth->modelCollision();
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
            //camera.moveLeft(timeElapsed * speed);
            cloth->moveAnchorPoints(vec3(-0.1f, 0.0f,0.0f));
        }

        if (key == GLFW_KEY_D) { // upper case!
            //camera.moveRight(timeElapsed * speed);
            cloth->moveAnchorPoints(vec3(0.1f, 0.0f,0.0f));
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
                //debug() << graph->values[i] << endl;
                debug() << to_string(cloth->getPosition()) << endl;
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
          sphere1->setPosition(sphere1->getPosition() + vec3(0.1f,0.0f,0.0f));
          debug() << to_string(sphere1->getPosition() - lowPolyMan->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_4) {
          sphere1->setPosition(sphere1->getPosition() - vec3(0.1f,0.0f,0.0f));
          debug() << to_string(sphere1->getPosition() - lowPolyMan->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_8) {
          sphere1->setPosition(sphere1->getPosition() + vec3(0.0f,0.1f,0.0f));
          debug() << to_string(sphere1->getPosition() - lowPolyMan->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_5) {
          sphere1->setPosition(sphere1->getPosition() - vec3(0.0f,0.1f,0.0f));
          debug() << to_string(sphere1->getPosition() - lowPolyMan->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_9) {
          sphere1->setPosition(sphere1->getPosition() + vec3(0.0f,0.0f,0.1f));
          debug() << to_string(sphere1->getPosition() - lowPolyMan->getPosition()) << endl;
        }
        if (key == GLFW_KEY_KP_6) {
          sphere1->setPosition(sphere1->getPosition() - vec3(0.0f,0.0f,0.1f));
          debug() << to_string(sphere1->getPosition() - lowPolyMan->getPosition()) << endl;
        }
    }
}

void PlayState::handleResizeEvents(GLFWwindow *window, glm::uvec2 windowSize) {
    camera.resize(windowSize.x, windowSize.y);
}
