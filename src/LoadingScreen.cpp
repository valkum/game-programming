#include "LoadingScreen.hh"
#include "Game.hh"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>
#include <ACGL/Utils/Log.hh>


#include <chrono>
#include <thread>


class LoadingScreen;

LoadingScreen::LoadingScreen() {
  // float quadVertices[] = {
  //   -1.f, -1.f, 0.f,
  //   -1.f, 1.f, 0.f,
  //   1.f, -1.f, 0.f,
  //   1.f, 1.f, 0.f
  // };

  // ACGL::OpenGL::SharedArrayBuffer fullQuadAB = ACGL::OpenGL::SharedArrayBuffer(new ACGL::OpenGL::ArrayBuffer());
  // fullQuadAB->defineAttribute("aPosition", GL_FLOAT, 3);
  // fullQuadAB->setDataElements(4, quadVertices);

  std::vector<glm::vec3> progressVertices= getProgressbarVertices(0);
  progressAB = ACGL::OpenGL::SharedArrayBuffer(new ACGL::OpenGL::ArrayBuffer());
  progressAB->defineAttribute("aPosition", GL_FLOAT, 3);
  progressAB->setDataElements(progressVertices.size(), value_ptr(progressVertices[0]), GL_DYNAMIC_DRAW);


  // backgroundVAO = ACGL::OpenGL::SharedVertexArrayObject(new ACGL::OpenGL::VertexArrayObject());
  // backgroundVAO->attachAllAttributes(fullQuadAB);
  // backgroundVAO->setMode(GL_TRIANGLE_STRIP);

  progressVAO = ACGL::OpenGL::SharedVertexArrayObject(new ACGL::OpenGL::VertexArrayObject());
  progressVAO->attachAllAttributes(progressAB);
  progressVAO->setMode(GL_TRIANGLE_STRIP);

  shader = ACGL::OpenGL::ShaderProgramCreator("loadingScreen").attributeLocations(progressVAO->getAttributeLocations()).create();
}

std::vector<glm::vec3> LoadingScreen::getProgressbarVertices(float value) {
  //CGame *game = CGame::instance();
  std::vector<glm::vec3> vertices;
  vertices.push_back(glm::vec3(-1.f, -1.f, 0.0f));
  vertices.push_back(glm::vec3(-1.f, -0.98f, 0.f));
  vertices.push_back(glm::vec3(-1.f + (value * 2), -1.f, 0.f));
  vertices.push_back(glm::vec3(-1.f + (value * 2), -0.98f, 0.f));
  return vertices;

}
void LoadingScreen::render(float value) {
    CGame *game = CGame::instance();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shader->use();
    shader->setUniform("uTime", 0.f);
    // glm:mat4 modelMatrix = glm::translate(glm::vec3(0.f, 0.f, 0.f));
    // shader->setUniform("uModelmatrix", modelMatrix);
    
    //shader->setUniform("uColor", glm::vec3(1.f, 1.f, 1.f));
    //backgroundVAO->render();

    std::vector<glm::vec3> progressVertices = getProgressbarVertices(value);
    progressAB->setSubData(0, progressVertices.size() * sizeof(progressVertices[0]), progressVertices.data());

    // modelMatrix = glm::translate(glm::vec3(2.f, -2.f, 0.f)) * triangleRotation;
    // shader->setUniform("uModelmatrix", modelMatrix);
    shader->setUniform("uColor", glm::vec3(0.f, 0.f, 0.f));
    progressVAO->render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwSwapBuffers(game->g_window);
    openGLCriticalError();
}
