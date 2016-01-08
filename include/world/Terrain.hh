#ifndef TERRAIN_H
#define TERRAIN_H
#include <glm/glm.hpp>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Types.hh>
//#include <ACGL/OpenGL/GL.hh>
//#include <GLFW/glfw3.h>

class Terrain{
public:

  void init();
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 
*viewProjectionMatrix);

private:

  ACGL::OpenGL::SharedVertexArrayObject vao;
  int mapSize;
  int height[][8];

};

#endif
