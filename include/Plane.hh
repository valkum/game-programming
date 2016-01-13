#pragma once
#include <glm/glm.hpp>

class Plane{
    public:

        vec3 n;
        //unit normal

        float d;
        //distance from the plane to the origin from a
        //normal and a point 

        //from 3 points
        Plane( const vec3& p0, const vec3& n ): N(n), D(-N.dot(p0)){}

        //signed distance from the plane topoint 'p' along
        //the unit normal
        Plane( const vec3& p0, const vec3& p1,
                const vec3& p2 ): N((p1-p0).cross(p2-p0).unit()),
        D(-N.dot(p0)){}

        const float distanceToPoint( const vec3& p ) const
        {
            return N.dot(p) + D;
        }
}
