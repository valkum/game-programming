#include "world/Cloud.hh"

#include <glm/glm.hpp>
#include <ACGL/OpenGL/Creator/Texture2DCreator.hh>
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/Utils/Log.hh>
#include <glm/gtc/random.hpp>
#include <glm/gtc/noise.hpp>
#include <math.h>

using namespace ACGL::OpenGL;
using namespace ACGL::Utils;
using namespace std;


Cloud::Cloud(uint_t amount) : Entity(vec3(0.f, 0.f, -1.f), vec3(0.f)), amount(amount) {
  cloudTex = ACGL::OpenGL::Texture2DFileManager::the()->get(ACGL::OpenGL::Texture2DCreator("cloud_particle.png"));
  init();
}

Cloud::Cloud(uint_t amount, vec3 pos) : Entity(pos, vec3(0.f)), amount(amount) {
  cloudTex = ACGL::OpenGL::Texture2DFileManager::the()->get(ACGL::OpenGL::Texture2DCreator("cloud_particle.png"));
  init();
}

void Cloud::init() {
  for (uint_t i = 0; i < amount; ++i) {
    particles.push_back(CloudParticle());
  }

  // spawn(25);
  // for (float i = 0.0f; i < 15.0f; i+=0.5f)
  // {
  //   for (float j = 0.0f; j < 15.0f; j+=0.5f)
  //   {
  //     for (float k = 5.0f; k < 7.5f; k+=0.5f)
  //     {
  //       float noise = (glm::perlin(vec3(i, k, j)) + 2) / 3.0f;
  //       debug() << noise << endl;
  //       respawnParticle(particles[firstUnusedParticle()], vec3(i, k, j), 1.0f, vec3(0.0f), noise);
  //     }
  //   }
  // }
  vec3 lastPos = position;
  for (uint_t i = 0; i < amount; ++i) {
    CloudParticle &p = particles[firstUnusedParticle()];
    respawnParticle(p, lastPos, 1.0f, vec3(0), 1.0f, linearRand(10.0f, 15.0f));
    lastPos = p.Position;
    if (glm::distance(position, lastPos) > 3.0f || distance(position.y, lastPos.y) > 1.5f) lastPos = position;
  }
  smooth();
}

void Cloud::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix, ACGL::OpenGL::SharedVertexArrayObject vao) {
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  shader->setUniform("uMVP", (*viewProjectionMatrix));

  for (CloudParticle particle : particles)
  {
      if (particle.Life > 0.0f)
      {
          shader->setUniform("uOffset", particle.Position);
          shader->setUniform("uScale", particle.Scale);
          shader->setUniform("uColor", particle.Color);
          shader->setTexture("uTexture", cloudTex, 3);
          
          vao->render();
      }
  }
  // Don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Cloud::spawn(uint_t num) {
  for(uint_t i = 0; i < num; ++i) {
    respawnParticle(particles[firstUnusedParticle()], position); 
  }
}

void Cloud::smooth() {
  float distance = 0.0f;
  float neighbors;
  // Update all particles
  for (uint_t i = 0; i < this->amount; ++i)
  {
      CloudParticle &p = this->particles[i];
      if (p.Life > 0.0f) // particle is alive, thus update
      {
          neighbors = 0;
          for (uint_t j = 0; j < this->amount; ++j) // calculate liveness based on neighboring particles
          {
              CloudParticle &n = this->particles[j];
              if (n.Life > 0.0f) // online live neighbors are relevant
              {
                  distance = glm::distance(n.Position, p.Position);
                  if (distance > 0 && distance <= 0.75f){
                      neighbors += 1/distance;
                  }
              }
          }
          p.Life = neighbors / 10.0f;
          // debug() << "before: " << p.Life << endl;
          if (p.Life < 0.1f) { // Life in range 0.5 to 1.0
              p.Life = 0.1f;
          } else if (p.Life > 1.0f) {
              p.Life = 1.0f;
          }
          // debug() << "after: " << p.Life << endl;
          // p.Color.a -= tanh(p.Life);
          p.Color.a *= 0.25f * p.Life;
      }
  }
}

void Cloud::update(float dt) {
  // Update all particles
  for (uint_t i = 0; i < this->amount; ++i)
  {
      CloudParticle &p = this->particles[i];
      if (p.Life > 0.0f) // particle is alive, thus update
      {
          p.Position += p.Velocity * dt;
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
    debug()<<"All used"<<std::endl;
    lastUsedParticle = 0;
    return 0;
}

void Cloud::respawnParticle(CloudParticle &particle, vec3 pos, float randomOffset, glm::vec3 offset, float alpha, float life, vec3 velocity) {
    glm::vec3 random = sphericalRand(randomOffset);
    float rColor = 0.4 + ((rand() % 100) / 100.0f);
    particle.Position = pos + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, alpha);
    particle.Life = life;
    particle.Scale = glm::scale(vec3(particle.Life));
    particle.Velocity = velocity;
}
