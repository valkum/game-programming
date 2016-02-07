#include "world/Clouds.hh"

#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <ACGL/OpenGL/Creator/Texture2DCreator.hh>
#include <ACGL/OpenGL/Managers.hh>
#include <algorithm>

using namespace ACGL::OpenGL;
using namespace ACGL::Utils;
using namespace std;

// CloudParticle: aPosition and aColor in one vector.
// static GLfloat particle_quad[6*5] = {
//       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
//       0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
//       -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 

//       0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
//       0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
//       -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 
//   }; 

Clouds::Clouds (uint_t amount, uint_t cloudSize, int width, int length) : cloudSize(cloudSize), particleAmount(amount*cloudSize), levelWidth(width), levelLength(length) {
  //alloc particles & GRAM
  particles.reserve(particleAmount);
  particleData.reserve(particleAmount);
  depthSort.reserve(particleAmount);

  for (uint_t i = 0; i < this->particleAmount; ++i) {
    particles.push_back(CloudParticle());
    // Fülle initial die lokale Kopie des ArrayBuffers mit max anzahl an particle Position, damit
    // auf der GPU genug speicher allokiert wird.
    Data data = {particles.back().Position, vec4(1.0f)};
    particleData.push_back(data);
  }

  // Wird nurnoch für den Shaderinit in PlayState::init() benutzt.
  ab = SharedOurArrayBuffer(new OurArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aRGBA", GL_FLOAT, 4);
  ab->setDataElements(particleData.capacity(), particleData.data(), GL_STREAM_DRAW);

  vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->setMode(GL_POINTS);
  vao->attachAllAttributes(ab);

  std::cout << ab->getSize() << std::endl;

  cloudTex = ACGL::OpenGL::Texture2DFileManager::the()->get(ACGL::OpenGL::Texture2DCreator("cloud_particle.png"));

  half = (width/2.0f);
	for (uint_t i = 0; i < amount; ++i)
	{
		spawnCloud(cloudSize, std::max(-half, -25.0f), std::min((float)width,50.0f), 0.0f, std::min((float)viewDistance, (float)length));
	}

  //alloc sort grid
  max_x = floor(levelWidth/(gridCellSize))-1;
  max_z = floor(levelLength/(gridCellSize))-1;
  grid = new vector<CloudParticle*>[(max_x+1)*(max_z+1)];
}


//spawns one cloud of $size particles placed randomly inside a box defined by (x,y,z) and (x+width,y+height,z+length)
void Clouds::spawnCloud(uint_t size, float x, float width, float z, float length, float y, float height) {
		//determine cloud center
		//todo: adjust with global wind vector and flying speed, so the cloud is at the position when char arrives
		x = glm::linearRand(x, x+width);
		y = glm::linearRand(y, y+height);
		z = glm::linearRand(z, z+length);
    debug()<<"spawning cloud at "<<x<<" "<<y<<" "<<z<<endl;

		//spawn particles around center
		vec3 position = vec3(x, y, z);
		vec3 lastPos = position;

    /* DEBUG: print map of particle usage
    for (uint_t j = 0; j < particleAmount/cloudSize; ++j) { uint_t count = 0;
    for (uint_t i = 0; i < cloudSize; ++i) {debug() << particles[(j*cloudSize)+i].Life; count += particles[(j*cloudSize)+i].Life;}
    debug()<<" "<<count<<endl;
    }*/

  	for (uint_t i = 0; i < size; ++i) {
    	CloudParticle &p = particles[firstUnusedParticle()];
   		respawnParticle(p, lastPos, 0.0f, vec3(linearRand(0.0f, 1.0f), linearRand(0.0f, 0.4f), linearRand(0.0f, 1.0f)), 1.0f, 1.0f/*linearRand(10.0f, 15.0f)*/);
   		lastPos = p.Position;
   		if (abs(glm::distance(position, lastPos)) > 2.0f || distance(position.y, lastPos.y) > .4f) lastPos = position;
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
    debug()<<"All particles used: " << deadParticleAmount << "/" << cloudSize <<std::endl;
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

bool ceparticle_sort(std::pair<float, CloudParticle*> *p0, std::pair<float, CloudParticle*> *p1){
  return (p0->first < p1->first);
}

void Clouds::update(float dt, vec3 camPos, glm::mat4 viewProjectionMatrix, vec3 wind){
  //clear grid
  for (uint_t i = 0; i < (max_x+1)*(max_z+1); i++) {
    grid[i].clear();
    // debug()<<"clear - size: "<<grid[i].size()<<endl;
  }

  //respawn dead clouds
  while(deadParticleAmount > cloudSize && camPos.z+(float)viewDistance < levelLength) {
    debug()<<"spawning, free: " << deadParticleAmount << "/" << cloudSize <<std::endl;
    spawnCloud(cloudSize, camPos.x-25.0f, 50.0f, camPos.z+(float)viewDistance, 0.0f);
    deadParticleAmount -= cloudSize;
  }

  // debug()<<"allocated "<<max_x*max_z<<endl;
  uint_t x;
  uint_t z;

  for (CloudParticle& particle : particles)
  {
    if ((particle.Position.z < (camPos.z - 5) || abs(particle.Position.x) > 1.5f * half) && particle.Life > 0.0f)
    { //kill particles in back of camera
      particle.Life = 0.0f;
      deadParticleAmount++;
      // debug()<<"free: " << deadParticleAmount << "/" << cloudSize <<std::endl;
    } else if (particle.Life > 0.0f) { //put particles in front into grid
      x = std::max(std::min(((uint_t)floor(floor(particle.Position.x+half)/(gridCellSize))),max_x),(uint_t)0); //assign x grid coord in [0, levelWidth]
      z = std::max(std::min(((uint_t)floor(floor(particle.Position.z)/(gridCellSize))),max_z),(uint_t)0); //assign z grid coord in [0, levelLength]
      // debug()<<"x->x/max_x "<<particle.Position.x<<"->"<<x<<"/"<<max_x<<endl;
      // debug()<<"x->z/max_z "<<particle.Position.z<<"->"<<z<<"/"<<max_z<<endl;
      // debug()<<"addr "<<x+(max_x*z)<<endl;
      grid[x+(max_x*z)].push_back(&particle);

      particle.Velocity *= 0.1f; //decay flow from last tick. this is done here already to avoid collision with viscosity code
    }
  }

  //declare stuff to alloc mem
  float dist = 0;
  vec3 direction = vec3(0);
  vec3 sumVel = vec3(0);
  float sumDist = 0;
  uint_t sumNo = 0;
  for (CloudParticle& particle : particles)
  {
    if (particle.Life > 0.0f)
    {
      //calculate grid position
      x = std::max(std::min(((uint_t)floor(floor(particle.Position.x+half)/(gridCellSize))),max_x),(uint_t)0); //assign x grid coord in [0, levelWidth]
      z = std::max(std::min(((uint_t)floor(floor(particle.Position.z)/(gridCellSize))),max_z),(uint_t)0); //assign z grid coord in [0, levelLength]
      //char collision:
      dist = distance(particle.Position, camPos+vec3(0,0,5));
      direction = particle.Position - (camPos+vec3(0,0,2));
      direction.y=0.0f;
      if(dist < 1.0f) particle.Velocity += direction/(5*dist);

      //environment collision

      if(glm::length(particle.Velocity)>0.0001f)
      {
        //viscosity
        /* relevant cells
        x+max_x-1 x+max_x x+max_x+1
        x-1 x x x+1
        x-max_x-1 x-max_x x-max_x+1

        foreach (exclusing self) in grid calculate distance
        if distance<viscRange insert std::pair(viscRange-distance, ptr) into vector<pair>
        sum directions and -distances
        weight???
        apply???
        */
        //calculate cells to check
        gridIDs.clear();
        // debug()<<"pos "<<to_string(particle.Position)<<endl;
        // debug()<<"x/z "<<x<<"/"<<z<<endl;
        if (x > 0) //left 3
        {
          if (z < max_z) //check front
          {
            gridIDs.push_back(x-1+(max_x*(z+1))); //front left
          }
          gridIDs.push_back(x-1+(max_x*z)); //left
          if (z > 0) //check back
          {
            gridIDs.push_back(x-1+(max_x*(z-1))); //back left
          }
        }
        if (z < max_z) //check front
        {
          gridIDs.push_back(x+(max_x*(z+1))); //front
        }
        gridIDs.push_back(x+(max_x*z)); //self
        if (z > 0) //check back
        {
          gridIDs.push_back(x+(max_x*(z-1))); //front
        }
        if (x < max_x) //right 3
        {
          if (z < max_z) //check front
          {
            gridIDs.push_back(x+1+(max_x*(z+1))); //front right
          }
          gridIDs.push_back(x+1+(max_x*z)); //right
          if (z > 0) //check back
          {
            gridIDs.push_back(x+1+(max_x*(z-1))); //back right
          }
        }

        //calculate neighborhood flow
        sumVel = vec3(0);
        sumDist = 0;
        sumNo = 0;
  			for (int gridID : gridIDs)
        {
          // debug()<<"ID "<<gridID;
          // debug()<<" size "<<grid[gridID].size()<<endl;
          for (CloudParticle* neighbor : grid[gridID])
          {
            if (&particle != neighbor)
            {
              dist = distance(particle.Position, neighbor->Position);
              if (dist < viscRange)
              {
                sumVel += neighbor->Velocity;
                sumDist += dist;
                sumNo++;
              }
            }
          }
        }
        // if(sumVel.z>0){ //debug
        //   debug()<<to_string(sumVel)<<endl;
        //   debug()<<sumDist<<endl;
        //   debug()<<sumNo<<endl;
        // }
        particle.Velocity = ((4*particle.Velocity) + (sumVel/sumDist))/5.0f;
        particle.Velocity = min((particle.Velocity / glm::length(particle.Velocity))/2.0f, particle.Velocity);
      }

      //update pos
			particle.Position += (2*particle.Velocity + wind) * dt; //update position on velocity including wind without saving wind
		}
	}

  // sort into new collection using distance to camera for depth-test
  depthSort.clear();
  //clear VRAM buffer
  particleData.clear();
  for (CloudParticle& particle : particles)
  {
    if (particle.Life > 0.0f)
    {
      depthSort.push_back(std::pair<float, CloudParticle*>((viewProjectionMatrix*vec4(particle.Position,1)).z,&particle));
      //depthSort.push_back(std::pair<float, CloudParticle*>(glm::length(particle.Position - camPos),&particle));
    }
  }
  std::sort(depthSort.begin(), depthSort.end());

  if (depthSort.size() > 0) { //assert non-null
    // render farthest particle first up to certain distance
    for (auto rit = depthSort.rbegin(); rit != depthSort.rend(); rit++)
    { 
      Data data;
      if(rit->first>3.0f) {
        data = {rit->second->Position, vec4(1)};
      } else {
        rit->second->Color.a = (glm::distance(camPos,rit->second->Position)/3.4f)+0.1f;
        data = {rit->second->Position, rit->second->Color};
      }
      particleData.push_back(data);
    }
    //apply alpha to counter view obstruction
    // for (; rit != depthSort.rend(); rit++)
    // {
    //   rit->second->Color.a = (glm::distance(camPos,rit->second->Position)/3.4f)+0.1f;
    //   Data data = {rit->second->Position, rit->second->Color};
    //   particleData.push_back(data);
    // }
  }

  // Sendet die aktuellen Positionen der Particle an die Graka. Da diese sich nur beim update ändern,
  // findet das update der particleData auch dort statt.
  // @TODO: Ggf muss hier mapRange verwendet werden, damit auf der Graka auch alte points geflusht werden.
  ab->setSize(particleData.size() * sizeof(particleData[0]));
  ab->setSubData(0, particleData.size() * sizeof(particleData[0]), particleData.data());
}

void Clouds::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix) {
  // Use additive blending to give it a 'glow' effect
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // shader->setUniform("uMVP", (*viewProjectionMatrix));

  glm::mat4 modelMatrix = translate(vec3(0.f)) * scale<float>(vec3(1.f));
  shader->setUniform("uModelMatrix", modelMatrix);
  shader->setUniform("uModelViewMatrix", (*viewMatrix) * modelMatrix);
  shader->setUniform("uMVP", (*projectionMatrix) * (*viewMatrix) * modelMatrix);
  shader->setUniform("uSize", vec2(0.5, 0.5));


  // Setze die Textur für den gerade ausgewählten shader. Nur einmal nötig. Pro Shader->use, bzw texture change.
  shader->setTexture("uTexture", cloudTex, 4);
  vao->render();
  // Don't forget to reset to default blending mode
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
