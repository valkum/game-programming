#include "world/Cloud.hh"

#include <glm/glm.hpp>
#include <ACGL/OpenGL/Creator/Texture2DCreator.hh>
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/Utils/Log.hh>
#include <glm/gtc/random.hpp>


using namespace ACGL::OpenGL;
using namespace ACGL::Utils;
using namespace std;

  // Particle: aPosition and aColor in one vector.
static GLfloat particle_quad[6*5] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 

      0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 
  }; 


Cloud::Cloud(uint_t amount) : Entity(vec3(0.f, 0.f, -1.f), vec3(0.f)), amount(amount) {
  texture = ACGL::OpenGL::Texture2DFileManager::the()->get(ACGL::OpenGL::Texture2DCreator(
                                                             "box.png"));
  init();
}

void Cloud::init() {
  ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aTexCoord", GL_FLOAT, 2);
  ab->setDataElements(6, particle_quad);

  vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);


  for (uint_t i = 0; i < nr_particles; ++i) {
    particles.push_back(Particle());
  }
}

void Cloud::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectioMatrix) {
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  shader->use();
  mat4 mvp = (*viewProjectioMatrix) * glm::scale(vec3(.2f));
  shader->setUniform("uMVP", mvp);

  for (Particle particle : particles)
  {
      if (particle.Life > 0.0f)
      {
          shader->setUniform("uOffset", particle.Position);
          shader->setUniform("uColor", particle.Color);
          
          vao->render();
      }
  }
  // Don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Cloud::update(float dt, uint_t newParticles) {
  // Add new particles 
  for (uint_t i = 0; i < newParticles; ++i)
  {
      int unusedParticle = firstUnusedParticle();
      respawnParticle(particles[unusedParticle], vec3(0));
  }
  // Update all particles
  for (uint_t i = 0; i < this->amount; ++i)
  {
      Particle &p = this->particles[i];
      p.Life -= dt; // reduce life
      if (p.Life > 0.0f)
      { // particle is alive, thus update
          p.Position += p.Velocity * dt; 
          p.Color.a -= dt * 1.5;
      }
  }
}

uint_t Cloud::firstUnusedParticle() {
    // First search from last used particle, this will usually return almost instantly
    for (uint_t i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (uint_t i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    //debug()<<"All used"<<std::endl;
    lastUsedParticle = 0;
    return 0;
}

void Cloud::respawnParticle(Particle &particle, glm::vec3 offset) {
    glm::vec3 random = sphericalRand(1.0f);
    float rColor = 0.4 + ((rand() % 100) / 100.0f);
    particle.Position = position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = vec3(0,1,0) * 10.0f;
}