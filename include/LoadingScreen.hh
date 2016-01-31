#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <vector>
#include <glm/glm.hpp>
#include <ACGL/OpenGL/Objects.hh>

class LoadingScreen {
public:
  LoadingScreen();
  void render(float value);
  ACGL::OpenGL::SharedShaderProgram getShader() {return shader; };
private:
  std::vector<glm::vec3> getProgressbarVertices(float value);
  ACGL::OpenGL::SharedVertexArrayObject backgroundVAO;
  ACGL::OpenGL::SharedVertexArrayObject progressVAO;
  ACGL::OpenGL::SharedShaderProgram shader;
  ACGL::OpenGL::SharedArrayBuffer progressAB;
};

#endif // LOADING_SCREEN_H
