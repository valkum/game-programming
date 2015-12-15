#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

 using namespace glm;

class Particle {
	private:
		bool movable;
		float mass;
		vec3 pos;
		vec3 old_pos;
		vec3 acceleration;
		vec3 accumulated_normal;

	public:
		Particle(vec3 pos); 

		Particle();

		void addForce(vec3 f);

		//TODO timestep or not?
		void timeStep();

		vec3& getPos();
		void resetAccerlation();
		void offsetPos(const vec3 v);
		void makeUnmovable();
		void addToNormal(vec3 normal);
		vec3& getNormal();
		void resetNormal();
};

#endif
