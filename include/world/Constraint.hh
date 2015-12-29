#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <ACGL/Utils/Log.hh>
#include <glm/ext.hpp>
#include "world/Particle.hh"

using namespace ACGL;
using namespace glm;

class Constraint {
	private: 
		float rest_distance; //length between two particles in rest configuration

	public:
		Particle *p1, *p2;
		Constraint (Particle *p1, Particle *p2): p1(p1), p2(p2){
			vec3 vec = p1->getPos()-p2->getPos();
			rest_distance = vec.length();
		}
		void statisfyConstraint(){
			vec3 p1_to_p2 = p2->getPos()-p1->getPos();
			float current_distance = p1_to_p2.length();
			//offsetVector that moves p1 into a distance of rest_distance to p2
			vec3 correctionVector = p1_to_p2*(1-rest_distance/current_distance);
			//Half that to move p1 and p2
			vec3 correctionVectorHalf = correctionVector*0.5f;
			p1->offsetPos(correctionVectorHalf);
			//inverse for correct way
			p2->offsetPos(-correctionVectorHalf);
            //ACGL::Utils::debug()<<glm::to_string(p1_to_p2)<<std::endl;
		}
};

#endif
