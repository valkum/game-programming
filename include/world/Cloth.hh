#ifndef CLOTH_H
#define CLOTH_H

#include "Particle.hh"
#include "Constraint.hh"
#include "Vec3.hh"
#include <glm/glm.hpp>

using namespace glm;

class Cloth{
	private:

		int num_particles_width;
		int num_particles_height;

		std::vector<Particle> particles; 
		std::vector<Constraint> constraints; 

		Particle* getParticle(int x, int y);
		void makeConstraint(Particle *p1, Particle *p2);


		Vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3);

		void addWindForcesForTriangle(Particle *p1,Particle *p2,Particle *p3, const vec3 direction);

		void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 color);

	public:

		Cloth(float width, float height, int num_particles_width, int num_particles_height);

		void drawShaded();

		void timeStep();

		void addForce(const Vec3 direction);

		void windForce(const Vec3 direction);

		void ballCollision(const Vec3 center,const float radius );

		void doFrame();
};

#endif
