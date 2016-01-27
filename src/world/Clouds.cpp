#include "world/Clouds.hh"

#include <glm/gtc/random.hpp>
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

Clouds::Clouds(uint_t amount, uint_t size, int width, int length) {
  ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aTexCoord", GL_FLOAT, 2);
  ab->setDataElements(6, particle_quad);

  vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);

  float half = width/2.0f;
	for (uint_t i = 0; i < amount; ++i)
	{
		spawn(size, -half, (float)width, 0.0f, (float)length);
	}
}

void Clouds::spawn(uint_t size, float x, float width, float z, float length, float y, float height) {
		x = glm::linearRand(x, x+width);
		y = glm::linearRand(y, y+height);
		z = glm::linearRand(z, z+length);
		clouds.push_back(Cloud(size, vec3(x, y, z)));
}

void Clouds::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix) {
	for (Cloud cloud : clouds)
	{
		cloud.render(shader, viewProjectionMatrix, vao);
	}
}

void Clouds::update(float dt, vec3 wind){
	for (auto&& cloud : clouds)
	{
		cloud.update(dt, wind);
	}
}
