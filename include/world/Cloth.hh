#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Particle.hh"
#include "Constraint.hh"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef GLINCLUDED
#define GLINCLUDED
# include <ACGL/OpenGL/GL.hh>
# include <GLFW/glfw3.h>
#endif

using namespace glm;

struct Vertex {
            vec3 position;
            vec2 uv;
            vec3 normal;
        };

class Cloth {
	public:
	Cloth(float width, float height, int num_particles_width, int num_particles_height);

	void drawShaded();

	void timeStep();

	void addForce(const vec3 direction);

	void windForce(const vec3 direction);

	void ballCollision(const vec3 center,const float radius );

	void doFrame();

private:
	int num_particles_width;
	int num_particles_height;
    GLuint vertexBuffer;
    GLuint vertexArrayObject;
    int elementSize;

	std::vector<Particle> particles; 
	std::vector<Constraint> constraints; 

	Particle* getParticle(int x, int y);
	void makeConstraint(Particle *p1, Particle *p2);

	void insertTriangle(Particle *p1, const vec2 uv, std::vector<Vertex> &vertexData);

	vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3);

	void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 direction);

	void drawTriangle(Particle *p1, Particle *p2, Particle *p3);
};

#endif
