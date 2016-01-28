#ifndef CLOUDS_HH
#define CLOUDS_HH

#include "world/CloudParticle.hh"
#include "world/Cloud.hh"

#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Types.hh>

class Clouds{
public:
  Clouds(uint_t amount, uint_t size, int width, int length);
  ACGL::OpenGL::SharedVertexArrayObject getVao() { return vao; }
  ACGL::OpenGL::SharedArrayBuffer getAb() {return ab;}
  inline ACGL::OpenGL::SharedTexture2D getTexture() {
    return this->cloudTex;
  }
  void update(float dt, vec3 wind);
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix, glm::vec3 camPos);
protected:
private:
  ACGL::OpenGL::SharedVertexArrayObject vao;
  ACGL::OpenGL::SharedArrayBuffer ab;
  ACGL::OpenGL::SharedTexture2D cloudTex;
  uint_t particleAmount;
  uint_t lastUsedParticle = 0;
  std::vector<CloudParticle> particles;
  void spawnCloud(uint_t size = 100, float x = 0.0f, float width = 0.0f, float z = 0.0f, float length = 0.0f, float y = 2.0f, float height = 0.0f);
  uint_t firstUnusedParticle();
  void respawnParticle(CloudParticle &particle, vec3 pos, float randomOffset = 0.0f, glm::vec3 offset = vec3(0.0f), float alpha = 1.0f, float life = 10.0f, vec3 velocity = vec3(0));
  void smooth();
};

#endif // CLOUD_HH
