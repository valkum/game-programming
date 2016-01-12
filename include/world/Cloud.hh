#ifndef CLOUD_HH
#define CLOUD_HH

#include "world/Entity.hh"
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Types.hh>
#include "world/CloudParticle.hh"


class Cloud : public Entity {
public:
  Cloud(uint_t amount);
  ACGL::OpenGL::SharedVertexArrayObject getVao() { return vao; }
  ACGL::OpenGL::SharedArrayBuffer getAb() {return ab;}
  inline ACGL::OpenGL::SharedTexture2D getTexture() {
    return this->cloudTex;
  }
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix);
  void update(float dt);
  void init();
  uint_t firstUnusedParticle();
  // void respawnParticle(CloudParticle &particle, glm::vec3 offset);
  void respawnParticle(CloudParticle &particle, vec3 position = vec3(0.0f), float randomOffset = 0.0f, glm::vec3 offset = vec3(0.0f), float alpha = 1.0f, float life = 10.0f, vec3 velocity = vec3(1,0,1) * 0.1f);
  void spawn (uint_t nun);
protected:
private:
  uint_t amount;
  // Stores the index of the last particle used (for quick access to next dead particle)
  uint_t lastUsedParticle = 0;
  std::vector<CloudParticle> particles;
  ACGL::OpenGL::SharedVertexArrayObject vao;
  ACGL::OpenGL::SharedArrayBuffer ab;
  ACGL::OpenGL::SharedTexture2D cloudTex;
};


#endif // CLOUD_HH
