#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <ACGL/Utils/Log.hh>
#include <glm/ext.hpp>
#include <glm/gtx/fast_square_root.hpp>
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
			rest_distance = length(vec);
		}
		void statisfyConstraint(){
			// vector from p1 to p2
			vec3 p1_to_p2 = p2->getPos()-p1->getPos(); 
			// current distance between p1 and p2
			float current_distance = fastLength(p1_to_p2);
			// The offset vector that could moves p1 into a distance of rest_distance to p2

			// @TODO maybe better calculation? Division is costly
			vec3 correctionVector = p1_to_p2*(1 - rest_distance/current_distance); 
			// Lets make it half that length, so that we can move BOTH p1 and p2.
			vec3 correctionVectorHalf = correctionVector*0.5f; 
			// correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
			p1->offsetPos(correctionVectorHalf); 
			// we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.
			p2->offsetPos(-correctionVectorHalf); 
		}


};

#endif
