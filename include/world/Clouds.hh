#ifndef CLOUDS_HH
#define CLOUDS_HH

#include "world/CloudParticle.hh"
#include "world/Entity.hh"

#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Types.hh>
#include <vector>
#include <glm/glm.hpp>
#include "OurArrayBuffer.hh"

struct Data{
  vec3 pos;
  vec4 rgba;
};

class Clouds{
public:
  Clouds(uint_t amount, uint_t size, int width, int length);
  ACGL::OpenGL::SharedVertexArrayObject getVao() { return vao; }
  ACGL::OpenGL::SharedArrayBuffer getAb() {return ab;}
  inline ACGL::OpenGL::SharedTexture2D getTexture() {
    return this->cloudTex;
  }

  void update(float dt, vec3 camPos, glm::mat4 viewProjectionMatrix, vec3 wind);
  // void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix, glm::vec3 camPos);
  void render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix);
protected:
private:
  ACGL::OpenGL::SharedVertexArrayObject vao;
  ACGL::OpenGL::SharedOurArrayBuffer ab;
  ACGL::OpenGL::SharedTexture2D cloudTex;
  uint_t cloudSize;
  uint_t particleAmount;
  int levelWidth = 0;
  float half = 0;
  int levelLength = 0;
  int viewDistance = 50;
  uint_t lastUsedParticle = 0;
  std::vector<CloudParticle> particles;
  /* Hier sind die Position der Particles gespeichert zum einfachen senden an die Grafikkarte. 
  / Sozusagen kopie des ArrayBuffers. Ein Eintrag = ein Vertex.
  / Falls mehr als die Position pro Particle Nötig ist (ggf. Größe oder farbe.) muss das hier angepasst werden. */
  std::vector<Data> particleData;
  std::vector<std::pair<float, CloudParticle*>> depthSort;
  uint_t deadParticleAmount = 0;
  uint_t viscRange = 2; //define range influential for viscosity
  uint_t gridCellSize = viscRange; //experimental multiplier
  uint_t max_x;
  uint_t max_z;
  std::vector<CloudParticle*> *grid;
  std::vector<uint_t> gridIDs;
  void spawnCloud(uint_t size = 100, float x = 0.0f, float width = 0.0f, float z = 0.0f, float length = 0.0f, float y = 1.5f, float height = 0.5f);
  uint_t firstUnusedParticle();
  void respawnParticle(CloudParticle &particle, vec3 pos, float randomOffset = 0.0f, glm::vec3 offset = vec3(0.0f), float alpha = 1.0f, float life = 10.0f, vec3 velocity = vec3(0));
  void smooth(std::vector<CloudParticle> toSmooth);
};

#endif // CLOUD_HH
