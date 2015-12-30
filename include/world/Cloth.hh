#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Particle.hh"
#include "Constraint.hh"
#include "world/Entity.hh"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ACGL/OpenGL/Objects.hh>

#ifndef GLINCLUDED
#define GLINCLUDED
# include <ACGL/OpenGL/GL.hh>
# include <GLFW/glfw3.h>
#endif

using namespace glm;

struct Vertex {
            vec3 position;
            vec3 uv;
            vec3 normal;
        };

class Cloth : public Entity {
	public:
	Cloth(float width, float height, int num_particles_width, int num_particles_height);

	void render(ACGL::OpenGL::SharedShaderProgram shader,
    mat4                             *viewProjectionMatrix);

	void timeStep(float dt);

	void addForce(const vec3 direction);

	void windForce(const vec3 direction);

	void ballCollision(const vec3 center,const float radius );

	void doFrame();
  ACGL::OpenGL::SharedVertexArrayObject getVAO() { return vao; }

private:
	int num_particles_width;
	int num_particles_height;
  
  ACGL::OpenGL::SharedVertexArrayObject vao;
  ACGL::OpenGL::SharedArrayBuffer ab;

	std::vector<Particle> particles; 
	std::vector<Constraint> constraints; 

	Particle* getParticle(int x, int y);
	void makeConstraint(Particle *p1, Particle *p2);

	void insertTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 uv, std::vector<Vertex> &vertexData);

	vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3);

	void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 direction, int x, int y);

	void drawTriangle(Particle *p1, Particle *p2, Particle *p3);
};

#endif
