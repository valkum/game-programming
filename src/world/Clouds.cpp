#include "world/Clouds.hh"

#include <glm/gtc/random.hpp>
#include <ACGL/OpenGL/Creator/Texture2DCreator.hh>
#include <ACGL/OpenGL/Managers.hh>

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

Clouds::Clouds (uint_t amount, uint_t cloudSize, int width, int length) : particleAmount(amount*cloudSize) {
  ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aTexCoord", GL_FLOAT, 2);
  ab->setDataElements(6, particle_quad);

  vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);

  cloudTex = ACGL::OpenGL::Texture2DFileManager::the()->get(ACGL::OpenGL::Texture2DCreator("cloud_particle.png"));

  //alloc particles
  for (uint_t i = 0; i < this->particleAmount; ++i) {
    particles.push_back(CloudParticle());
  }

  float half = width/2.0f;
	for (uint_t i = 0; i < amount; ++i)
	{
		spawnCloud(cloudSize, -half, (float)width, 0.0f, (float)length);
	}
}

void Clouds::spawnCloud(uint_t size, float x, float width, float z, float length, float y, float height) {
		//determine cloud center
		//todo: adjust with global wind vector and flying speed, so the cloud is at the position when char arrives
		x = glm::linearRand(x, x+width);
		y = glm::linearRand(y, y+height);
		z = glm::linearRand(z, z+length);
		// clouds.push_back(Cloud(size, vec3(x, y, z)));

		//spawn particles around center
		vec3 position = vec3(x, y, z);
		vec3 lastPos = position;
  	for (uint_t i = 0; i < size; ++i) {
    	CloudParticle &p = particles[firstUnusedParticle()];
   		respawnParticle(p, lastPos, 0.0f, vec3(linearRand(0.0f, 1.0f), linearRand(0.0f, 0.4f), linearRand(0.0f, 1.0f)), 1.0f, 1.0f/*linearRand(10.0f, 15.0f)*/);
   		lastPos = p.Position;
   		if (abs(glm::distance(position, lastPos)) > 3.0f || distance(position.y, lastPos.y) > .4f) lastPos = position;
  	}
  	smooth();
}

uint_t Clouds::firstUnusedParticle() {
    // First search from last used particle, this will usually return almost instantly
    for (uint_t i = lastUsedParticle; i < particleAmount; ++i){
        if (particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (uint_t i = 0; i < lastUsedParticle; ++i){
        if (particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    debug()<<"All particles used"<<std::endl;
    lastUsedParticle = 0;
    return 0;
}

void Clouds::respawnParticle(CloudParticle &particle, vec3 pos, float randomOffset, glm::vec3 offset, float alpha, float life, vec3 velocity) {
    glm::vec3 random = sphericalRand(randomOffset);
    float rColor = 0.4 + ((rand() % 100) / 100.0f);
    particle.Position = pos + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, alpha);
    particle.Life = life;
    particle.Scale = glm::scale(vec3(particle.Life));
    particle.Velocity = velocity;
}

void Clouds::smooth() {
  float distance = 0.0f;
  float neighbors;
  // Update all particles
  for (uint_t i = 0; i < this->particleAmount; ++i)
  {
      CloudParticle &p = this->particles[i];
      if (p.Life > 0.0f) // particle is alive, thus update
      {
          neighbors = 0;
          //todo: optimize usingsearchgrid
          for (uint_t j = 0; j < this->particleAmount; ++j) // calculate liveness based on density of neighboring particles
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
          float density = neighbors / 10.0f;
          // debug() << "before: " << p.Life << endl;
          if (density < 0.25f) { // Life in range 0.5 to 1.0
              density = 0.25f;
          } else if (density > 1.0f) {
              density = 1.0f;
          }
          // debug() << "after: " << p.Life << endl;
          // p.Color.a -= tanh(p.Life);
          p.Color.a *= density;
      }
  }
}

void Clouds::update(float dt, vec3 wind){
	//sort() //include life?
	//die()
  for (CloudParticle& particle : particles)
  {
    if (particle.Life > 0.0f)
    {
			particle.Velocity = wind; //add global wind flow
			//collision (char & environment)
			//viscosity
			//update pos
			particle.Position += particle.Velocity * dt;
		}
	}
}

void Clouds::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix, glm::vec3 camPos) {
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  shader->setUniform("uMVP", (*viewProjectionMatrix));

  //sort into new collection using distance to camera for depth-test
  std::map<float, CloudParticle*> depthSort;
  for (CloudParticle& particle : particles)
  {
  	if (particle.Life > 0.0f)
    {
	  	depthSort.insert(std::pair<float, CloudParticle*>(glm::distance(camPos,particle.Position),&particle));
	  }
  }

  // traverse in reverse order
  for (auto rit = depthSort.rbegin(); rit != depthSort.rend(); rit++)
  {
  	// debug() << to_string(rit->first) << endl;
    shader->setUniform("uOffset", rit->second->Position);
    shader->setUniform("uScale", rit->second->Scale);
    shader->setUniform("uColor", rit->second->Color);
    shader->setTexture("uTexture", cloudTex, 3);
    vao->render();
  }
  // Don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
