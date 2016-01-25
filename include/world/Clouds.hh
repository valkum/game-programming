#ifndef CLOUDS_HH
#define CLOUDS_HH

#include "world/Cloud.hh"

class Clouds{
public:
  Clouds(uint_t amount, uint_t size);
  ACGL::OpenGL::SharedVertexArrayObject getVao() { return vao; }
  ACGL::OpenGL::SharedArrayBuffer getAb() {return ab;}
  inline ACGL::OpenGL::SharedTexture2D getTexture() {
    return clouds[0].getTexture();
  }
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix);
  void update(float dt, vec3 wind);
protected:
private:
  std::vector<Cloud> clouds;
  ACGL::OpenGL::SharedVertexArrayObject vao;
  ACGL::OpenGL::SharedArrayBuffer ab;
};

#endif // CLOUD_HH
