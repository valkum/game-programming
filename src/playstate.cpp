#include "playstate.h"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>
#include <ACGL/OpenGL/Creator/VertexArrayObjectCreator.hh>
#include <ACGL/OpenGL/Data/GeometryDataLoadStore.hh>
#include <ACGL/OpenGL/Data/TextureLoadStore.hh>
#include <ACGL/Scene/GenericCamera.hh>
#include "TextureLoadStore.h"
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Base/Settings.hh>
#include <ACGL/Math/Math.hh>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "helper.h"

using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;
using namespace ACGL::Scene;

PlayState PlayState::m_PlayState;
SharedShaderProgram boxShader;
SharedShaderProgram skyBoxShader;
SharedVertexArrayObject vaoCube, skyCube;

GLuint skyboxVAO, skyboxVBO;

vec3 gPosition1(0.0f, 0.0f, 0.0f);
vec3 gOrientation1;
SharedTextureCubeMap cubemapTexture;

GenericCamera camera;



// static const GLfloat g_vertex_buffer_data[] = {
//     -1.0f,-1.0f,-1.0f, // triangle 1 : begin
//     -1.0f,-1.0f, 1.0f,
//     -1.0f, 1.0f, 1.0f, // triangle 1 : end
//     1.0f, 1.0f,-1.0f, // triangle 2 : begin
//     -1.0f,-1.0f,-1.0f,
//     -1.0f, 1.0f,-1.0f, // triangle 2 : end
//     1.0f,-1.0f, 1.0f,
//     -1.0f,-1.0f,-1.0f,
//     1.0f,-1.0f,-1.0f,
//     1.0f, 1.0f,-1.0f,
//     1.0f,-1.0f,-1.0f,
//     -1.0f,-1.0f,-1.0f,
//     -1.0f,-1.0f,-1.0f,
//     -1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f,-1.0f,
//     1.0f,-1.0f, 1.0f,
//     -1.0f,-1.0f, 1.0f,
//     -1.0f,-1.0f,-1.0f,
//     -1.0f, 1.0f, 1.0f,
//     -1.0f,-1.0f, 1.0f,
//     1.0f,-1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f,-1.0f,-1.0f,
//     1.0f, 1.0f,-1.0f,
//     1.0f,-1.0f,-1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f,-1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f,-1.0f,
//     -1.0f, 1.0f,-1.0f,
//     1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f,-1.0f,
//     -1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f, 1.0f,
//     1.0f,-1.0f, 1.0f
// };

// static const GLfloat g_color_buffer_data[] = {
//     0.583f,  0.771f,  0.014f,
//     0.609f,  0.115f,  0.436f,
//     0.327f,  0.483f,  0.844f,
//     0.822f,  0.569f,  0.201f,
//     0.435f,  0.602f,  0.223f,
//     0.310f,  0.747f,  0.185f,
//     0.597f,  0.770f,  0.761f,
//     0.559f,  0.436f,  0.730f,
//     0.359f,  0.583f,  0.152f,
//     0.483f,  0.596f,  0.789f,
//     0.559f,  0.861f,  0.639f,
//     0.195f,  0.548f,  0.859f,
//     0.014f,  0.184f,  0.576f,
//     0.771f,  0.328f,  0.970f,
//     0.406f,  0.615f,  0.116f,
//     0.676f,  0.977f,  0.133f,
//     0.971f,  0.572f,  0.833f,
//     0.140f,  0.616f,  0.489f,
//     0.997f,  0.513f,  0.064f,
//     0.945f,  0.719f,  0.592f,
//     0.543f,  0.021f,  0.978f,
//     0.279f,  0.317f,  0.505f,
//     0.167f,  0.620f,  0.077f,
//     0.347f,  0.857f,  0.137f,
//     0.055f,  0.953f,  0.042f,
//     0.714f,  0.505f,  0.345f,
//     0.783f,  0.290f,  0.734f,
//     0.722f,  0.645f,  0.174f,
//     0.302f,  0.455f,  0.848f,
//     0.225f,  0.587f,  0.040f,
//     0.517f,  0.713f,  0.338f,
//     0.053f,  0.959f,  0.120f,
//     0.393f,  0.621f,  0.362f,
//     0.673f,  0.211f,  0.457f,
//     0.820f,  0.883f,  0.371f,
//     0.982f,  0.099f,  0.879f
// };
// static  GLfloat skyboxVertices[] = {
//     // Positions  
//     // x      y      z     w        
//     -1.0f,  1.0f, -1.0f, 1.0f,
//     -1.0f, -1.0f, -1.0f, 1.0f,
//      1.0f, -1.0f, -1.0f, 1.0f,
//      1.0f, -1.0f, -1.0f, 1.0f,
//      1.0f,  1.0f, -1.0f, 1.0f,
//     -1.0f,  1.0f, -1.0f, 1.0f,

//     -1.0f, -1.0f,  1.0f, 1.0f,
//     -1.0f, -1.0f, -1.0f, 1.0f,
//     -1.0f,  1.0f, -1.0f, 1.0f,
//     -1.0f,  1.0f, -1.0f, 1.0f,
//     -1.0f,  1.0f,  1.0f, 1.0f,
//     -1.0f, -1.0f,  1.0f, 1.0f,

//      1.0f, -1.0f, -1.0f, 1.0f,
//      1.0f, -1.0f,  1.0f, 1.0f,
//      1.0f,  1.0f,  1.0f, 1.0f,
//      1.0f,  1.0f,  1.0f, 1.0f,
//      1.0f,  1.0f, -1.0f, 1.0f,
//      1.0f, -1.0f, -1.0f, 1.0f,

//     -1.0f, -1.0f,  1.0f, 1.0f,
//     -1.0f,  1.0f,  1.0f, 1.0f,
//      1.0f,  1.0f,  1.0f, 1.0f,
//      1.0f,  1.0f,  1.0f, 1.0f,
//      1.0f, -1.0f,  1.0f, 1.0f,
//     -1.0f, -1.0f,  1.0f, 1.0f,

//     -1.0f,  1.0f, -1.0f, 1.0f,
//      1.0f,  1.0f, -1.0f, 1.0f,
//      1.0f,  1.0f,  1.0f, 1.0f,
//      1.0f,  1.0f,  1.0f, 1.0f,
//     -1.0f,  1.0f,  1.0f, 1.0f,
//     -1.0f,  1.0f, -1.0f, 1.0f,

//     -1.0f, -1.0f, -1.0f, 1.0f,
//     -1.0f, -1.0f,  1.0f, 1.0f,
//      1.0f, -1.0f, -1.0f, 1.0f,
//      1.0f, -1.0f, -1.0f, 1.0f,
//     -1.0f, -1.0f,  1.0f, 1.0f,
//      1.0f, -1.0f,  1.0f, 1.0f
// };
// static const glm::mat4 buildFrustum( float phiInDegree, float _near, float _far, float aspectRatio) {

//     float phiHalfInRadians = 0.5*phiInDegree * (M_PI/180.0);
//     float top = _near * tan( phiHalfInRadians );
//     float bottom = -top;
//     float left  = bottom * aspectRatio;
//     float right = -left;

//     return glm::frustum(left, right, bottom, top, _near, _far);
// }

void PlayState::Init(CGame* game) {


	
	// define where shaders and textures can be found:
    Settings::the()->setResourcePath(helper::getExePath() + "/assets/");
    Settings::the()->setShaderPath("shaders/");
    Settings::the()->setGeometryPath("geometry/");
    Settings::the()->setTexturePath("textures/");
	

    // construct VAO to give shader correct Attribute locations
    SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
    ab->defineAttribute("aPosition", GL_FLOAT, 3);
    ab->defineAttribute("aTexCoord", GL_FLOAT, 3);
    ab->defineAttribute("aNormal", GL_FLOAT, 3);

    SharedVertexArrayObject vao = SharedVertexArrayObject(new VertexArrayObject());
    vao->attachAllAttributes(ab);



    debug()<<"Loading geometry stage"<<endl;
    vaoCube = VertexArrayObjectCreator("cube.obj").create();
    //vaoCube->setMode(GL_LINE_LOOP);
    debug()<<"Geometry loaded"<<endl;

    debug()<<"Loading shaders state"<<endl;
	cout<<Settings::the()->getFullShaderPath()<<endl;
	boxShader = ShaderProgramCreator("cube").attributeLocations(
        vao->getAttributeLocations()).create();
    skyBoxShader = ShaderProgramCreator("skybox").attributeLocations(
        vao->getAttributeLocations()).create();

	debug()<<"Shaders loaded"<<endl;

    skyCube = VertexArrayObjectCreator("cube.obj").create();
    skyCube->setAttributeLocations( skyBoxShader->getAttributeLocations() );
    openGLCriticalError();
    //Texture
    std::vector<std::string> paths = {
        Settings::the()->getFullTexturePath() + "nuke_rt.png",
        Settings::the()->getFullTexturePath() + "nuke_lf.png",
        Settings::the()->getFullTexturePath() + "nuke_up.png",
        Settings::the()->getFullTexturePath() + "nuke_dn.png",
        Settings::the()->getFullTexturePath() + "nuke_bk.png",
        Settings::the()->getFullTexturePath() + "nuke_ft.png",
    };
    openGLCriticalError();
    cubemapTexture = loadTextureCubeMap(paths);
    skyBoxShader->use();
    skyBoxShader->setTexture("uTexture", cubemapTexture, 0);
    openGLCriticalError();
    debug()<<"Set Texture for skyBoxShader"<<endl;

    openGLCriticalError();
    vaoCube->bind();

    camera.setVerticalFieldOfView(90.0);

    glEnable(GL_DEPTH_TEST);

}	
void PlayState::Draw(CGame* g, float* delta) {

	//std::cout<<"Draw IntroState at time: "<<*delta<<std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    openGLCriticalError();
    // Remove any translation component of the view matrix
    // glm::mat4 projection = buildFrustum(75.0, 0.1, 100.0, (float)g->g_windowSize.x/(float)g->g_windowSize.y);

    glm::mat4 modelMatrix;
    
    boxShader->use();
    //glm::mat4 viewMatrix = glm::translate(glm::vec3(0.0f, -1.0f, -2.0f)) * glm::rotate<float>(1.0472f * (*delta), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale<float>(glm::vec3(0.25f));
    mat4 rotateMatrix = rotate(0.0f, vec3(1,0,0))* rotate(0.0f, vec3(0,1,0))*rotate(0.0f, vec3(0,0,1));
    modelMatrix = translate(vec3(0.0f,0.0f,0.0f)) * rotateMatrix * scale<float>(glm::vec3(1.0f));
    boxShader->setUniform( "uModelMatrix", modelMatrix);
    boxShader->setUniform( "uViewMatrix",  camera.getViewMatrix());
    boxShader->setUniform( "uProjectionMatrix", camera.getProjectionMatrix());
    boxShader->setUniform( "uNormalMatrix", camera.getRotationMatrix3() );
    openGLCriticalError();

    vaoCube->render();
    openGLCriticalError();


    // glDepthFunc(GL_LEQUAL);
    // skyBoxShader->use();
    // glm::mat4 projectionMatrix = camera.getProjectionMatrix();
    // skyBoxShader->setUniform("uProjectionMatrix", projectionMatrix);
    // glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.getViewMatrix()));
    // skyBoxShader->setUniform("uViewMatrix", viewMatrix);
    // rotateMatrix = rotate(0.0f, vec3(1,0,0))* rotate(0.0f, vec3(0,1,0))*rotate(0.0f, vec3(0,0,1));
    // modelMatrix = translate(vec3(0.0f,0.0f,0.0f)) * rotateMatrix * scale<float>(glm::vec3(1.0f));
    // skyBoxShader->setUniform("uModelMatrix", modelMatrix);

    // glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    // skyBoxShader->setUniform("uModelViewProjectionMatrix", mvp);

    // skyBoxShader->setUniform("uCameraPosition", camera.getPosition());
    
    // skyCube->render();
    // glDepthFunc(GL_LESS);
    openGLCriticalError();


}

void PlayState::HandleMouseMoveEvents(GLFWwindow* window, glm::vec2 mousePos){

}
void PlayState::HandleMouseButtonEvents(GLFWwindow* window, glm::vec2 mousePos, int button, int action, int mods){

}
void PlayState::HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods) {
    CGameState::HandleKeyEvents(window, key, scancode, action, mods);
    // make camera movements based on the elapsed time and not based on frames rendered!
    double timeElapsed = .05;

    double speed = 5.0; // magic value to scale the camera speed

    if(action == GLFW_PRESS | action == GLFW_REPEAT) {
        if (key == GLFW_KEY_W) { // upper case!
            camera.moveForward( timeElapsed*speed );
        }
        if (key == GLFW_KEY_Q) { // upper case!
            camera.FPSstyleLookAround( -timeElapsed*speed, 0);
        }
        if (key == GLFW_KEY_S) { // upper case!
            camera.moveBack( timeElapsed*speed );
        }
        if (key == GLFW_KEY_E) { // upper case!
            camera.FPSstyleLookAround( timeElapsed*speed, 0 );
        }
        if (key == GLFW_KEY_A) { // upper case!
            camera.moveLeft( timeElapsed*speed );
        }
        if (key == GLFW_KEY_D) { // upper case!
            camera.moveRight( timeElapsed*speed );
        }
    }
}
void PlayState::HandleResizeEvents(GLFWwindow * window, glm::uvec2 windowSize){
    camera.resize( windowSize.x, windowSize.y );
}