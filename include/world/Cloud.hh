#ifndef CLOUD_HH
#define CLOUD_HH

#include "world/Entity.hh"
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Types.hh>
#include "world/Particle.hh"


class Cloud : public Entity {
public:
  Cloud(uint_t amount);
  ACGL::OpenGL::SharedVertexArrayObject getVao() { return vao; }
  ACGL::OpenGL::SharedArrayBuffer getAb() {return ab;}
  inline ACGL::OpenGL::SharedTexture2D getTexture() {
    return this->texture;
  }
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix);
  void update(float dt, uint_t num);
  void init();
  uint_t firstUnusedParticle();
  void respawnParticle(Particle &particle, glm::vec3 offset);
protected:
private:
  uint_t amount;
  // Stores the index of the last particle used (for quick access to next dead particle)
  uint_t lastUsedParticle = 0;
  uint_t nr_particles = 500;
  std::vector<Particle> particles;
  ACGL::OpenGL::SharedVertexArrayObject vao;
  ACGL::OpenGL::SharedArrayBuffer ab;
  ACGL::OpenGL::SharedTexture2D texture;
};


#endif // CLOUD_HH
