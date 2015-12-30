#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
//#define TIME_STEPSIZE2 0.25f*0.25f
// 1/64 Tickrate
#define TIME_STEPSIZE2 0.0156f
#define DAMPING 0.01f

using namespace glm;
class Particle {
	private:
		vec3 pos;
		vec3 old_pos;
		vec3 acceleration;
		float mass; //default mass = 1
		bool movable;//pin parts of the cloth
		vec3 accumulated_normal;//used for OpenGL soft shading

	public:
		Particle(vec3 pos) : pos(pos), old_pos(pos), acceleration(vec3(0,0,0)), mass(1), movable(true), accumulated_normal(vec3(0,0,0)){};

		Particle(){}

		void addForce(vec3 f){
			acceleration += f/mass;
		}

		//TODO timestep or not?
		void timeStep(float dt){
			if (movable) {
				vec3 temp = pos;
				//verlet integration
                //TODO timestep
				pos = pos+(pos-old_pos)*(1.0f-DAMPING)+acceleration*dt;
				old_pos = temp;
				acceleration = vec3(0,0,0);//Since the acceleration has been translated to a force, it has been reset.
			} 
		}

		vec3& getPos(){
			return pos;
		}
		void resetAccerlation(){
			acceleration = vec3(0,0,0);
		}
		void offsetPos(const vec3 v){
			if(movable){
				pos += v;
			}
		}
		void makeUnmovable(){
			movable = false;
		}
		void addToNormal(vec3 normal){
			accumulated_normal += normalize(normal);
		}
		bool getMovable(){
			return movable;
		}
		vec3& getNormal(){
			return accumulated_normal;
		}
		void resetNormal(){
			accumulated_normal = vec3(0,0,0);
		}
};


#endif
