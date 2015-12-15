#include "world/Vec3.hh"

class Particle {
	private:
		bool movable;//pin parts of the cloth
		float mass; //default mass = 1
		Vec3 pos;
		Vec3 old_pos;
		Vec3 acceleration;
		Vec3 accumulated_normal;//used for OpenGL soft shading

	public:
		Particle(Vec3 pos) : pos(pos), old_pos(pos), acceleration(Vec3(0,0,0)), mass(1), movable(true), accumulated_normal(Vec3(0,0,0)){}

		Particle(){}

		void addForce(Vec3 f){
			acceleration += f/mass;
		}

		//TODO timestep or not?
		void timeStep(){
			if (movable) {
				Vec3 temp = pos;
				//verlet integration
				pos = pos+(pos-old_pos)*(1.0-DAMPING)+acceleration*TIME_STEPSIZE2;
				old_pos = temp;
				acceleration = Vec3(0,0,0);//Since the acceleration has been translated to a force, it has been reset.
			} 
		}

		Vec3& getPos(){
			return pos;
		}
		void resetAccerlation(){
			acceleration = Vec3(0,0,0);
		}
		void offsetPos(const Vec3 v){
			if(movable){
				pos += v;
			}
		}
		void makeUnmovable(){
			movable = false;
		}
		void addToNormal(Vec3 normal){
			accumulated_normal += normal.normalized();
		}
		Vec3& getNormal(){
			return accumulated_normal;
		}
		void resetNormal(){
			accumulated_normal = Vec3(0,0,0);
		}
};
