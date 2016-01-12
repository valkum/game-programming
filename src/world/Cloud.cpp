#include "world/Cloud.hh"

#include <glm/glm.hpp>
#include <ACGL/OpenGL/Creator/Texture2DCreator.hh>
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/Utils/Log.hh>
#include <glm/gtc/random.hpp>
#include <math.h>


using namespace ACGL::OpenGL;
using namespace ACGL::Utils;
using namespace std;

  // CloudParticle: aPosition and aColor in one vector.
static GLfloat particle_quad[6*5] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 

      0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 
  }; 


Cloud::Cloud(uint_t amount) : Entity(vec3(0.f, 0.f, -1.f), vec3(0.f)), amount(amount) {
  cloudTex = ACGL::OpenGL::Texture2DFileManager::the()->get(ACGL::OpenGL::Texture2DCreator("cloud_particle.png"));
  nr_particles = amount;
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
    particles.push_back(CloudParticle());
  }

  spawn(100);
}

void Cloud::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix) {
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // shader->use();
  mat4 mvp = (*viewProjectionMatrix) * glm::scale(vec3(.2f));
  shader->setUniform("uMVP", mvp);

  for (CloudParticle particle : particles)
  {
      // if (particle.Life > 0.0f)
      // {
          shader->setUniform("uOffset", particle.Position);
          // shader->setUniform("uColor", particle.Color);
          shader->setTexture("uTexture", cloudTex, 3);
          
          vao->render();
      // }
  }
  // Don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Cloud::spawn(uint_t num) {
  for(uint_t i = 0; i < num; ++i) {
    int unusedParticle = firstUnusedParticle();
    respawnParticle(particles[unusedParticle], vec3(this->particles[i].Position.x+1,0 ,0)); 
  }
}

void Cloud::update(float dt) {
  // Update all particles
  for (uint_t i = 0; i < this->amount; ++i)
  {
      CloudParticle &p = this->particles[i];
      // p.Life -= dt; // reduce life
      // if (p.Life > 0.0f)
      // { // particle is alive, thus update
          // p.Position += p.Velocity * dt;
          p.Color.a -= tanh(p.Life);
      // }
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

void Cloud::respawnParticle(CloudParticle &particle, glm::vec3 offset) {
    glm::vec3 random = sphericalRand(1.0f);
    float rColor = 0.4 + ((rand() % 100) / 100.0f);
    particle.Position = position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 10.0f;
    particle.Velocity = vec3(0,1,0) * 1.0f;
}