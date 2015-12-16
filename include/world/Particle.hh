#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include "Vec3.hh"

 using namespace glm;

class Particle {
	private:
		bool movable;
		float mass;
		Vec3 pos;
		Vec3 old_pos;
		Vec3 acceleration;
		Vec3 accumulated_normal;

	public:
		Particle(Vec3 pos); 

		Particle();

		void addForce(Vec3 f);

		//TODO timestep or not?
		void timeStep();

		Vec3& getPos();
		void resetAccerlation();
		void offsetPos(const Vec3 v);
		void makeUnmovable();
		void addToNormal(Vec3 normal);
		Vec3& getNormal();
		void resetNormal();
};

#endif
