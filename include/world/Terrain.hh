#ifndef TERRAIN_H
#define TERRAIN_H
#include <glm/glm.hpp>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Types.hh>
#include <glm/gtc/type_ptr.hpp>

class Terrain{
public:
  struct Vertex {
            glm::tvec3<float> position;
            glm::tvec3<float> normal;
        };
  Terrain(int x, int y, float a = 100.f, float b = 6.f);
  ACGL::OpenGL::SharedVertexArrayObject getVAO() { return vao; };
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 
*viewProjectionMatrix);

private:
  size_t index( int w, int l ) const { return w * length + l; }
  float* generate(float a, float b);
  void insertQuad(std::vector<Vertex> &vertexData, float* height, int x, int z);
  glm::vec3 calcTriangleNormal(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3);
  ACGL::OpenGL::SharedVertexArrayObject vao;
  int width;
  int length;
  float* height;
};

#endif
