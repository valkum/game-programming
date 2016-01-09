#ifndef TERRAIN_H
#define TERRAIN_H
#include <glm/glm.hpp>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Types.hh>
#include <glm/gtc/type_ptr.hpp>
//#include <ACGL/OpenGL/GL.hh>
//#include <GLFW/glfw3.h>

class Terrain{
public:
  Terrain();
  ACGL::OpenGL::SharedVertexArrayObject getVAO() { return vao; };
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 
*viewProjectionMatrix);

private:

  ACGL::OpenGL::SharedVertexArrayObject vao;

};

#endif
