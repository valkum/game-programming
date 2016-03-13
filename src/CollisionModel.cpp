#include "CollisionModel.hh"
#include "Plane.hh"

using namespace std;

class CollisionModel;

CollisionModel::CollisionModel(Mode model, vec3 position, vec3 rotation): Entity(
        position,
        rotation){
    this->model = model;
}

CollisionModel::CollisionModel(){}

CollisionModel::~CollisionModel(){}

//void CollisionModel::update(vec3 position, vec3 rotation)

bool checkCollision(vec3 point){

};

bool PlaneCollision(const float r, //sphere radius
                            const vec3& C0, //previous position of sphere
                            const vec3& C1, //current position of sphere
                            const Plane& plane, //the plane
                            vec3& Ci, //position of sphere when it first touched the plane
                            float& u //normalized time of collision
){
    const float d0 = plane.distanceToPoint( C0 );
    const float d1 = plane.distanceToPoint( C1 );

    //check if it was touching on previous frame
    if( fabs(d0) <= r )
    {
        Ci = C0;
        u = 0;
        return true; 

    }

    //check if the sphere penetrated during this frame
    if( d0>r && d1<r )
    {
        u = (d0-r)/(d0-d1); //normalized time
        Ci = (1-u)*C0 + u*C1; //point of first contact
        return true; 
    }

    return false;
} 
