#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Particle.hh"
#include "Constraint.hh"
#include "Vec3.hh"
#include <glm/glm.hpp>
#include <GL/gl.h>

using namespace glm;

class Cloth {
	public:
	Cloth(float width, float height, int num_particles_width, int num_particles_height);

	void drawShaded();

	void timeStep();

	void addForce(const Vec3 direction);

	void windForce(const Vec3 direction);

	void ballCollision(const Vec3 center,const float radius );

	void doFrame();

private:
	int num_particles_width;
	int num_particles_height;
    GLuint vertexbuffer;

	std::vector<Particle> particles; 
	std::vector<Constraint> constraints; 

	Particle* getParticle(int x, int y);
	void makeConstraint(Particle *p1, Particle *p2);


	Vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3);

	void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 direction);

	void drawTriangle(Particle *p1, Particle *p2, Particle *p3);
};

#endif
