#include "world/Cloth.hh"
#include <ACGL/OpenGL/GL.hh>
#include <ACGL/Utils/Log.hh>


#define CONSTRAINT_ITERATIONS 5

using namespace std;
using namespace glm;
using namespace ACGL::OpenGL;
using namespace ACGL::Utils;
class Cloth;

float rad0 = 3;
float rad1 = 4;
float rad2 = 6;
float rad3 = 4;
float rad4 = 4;

vec3 sphereOffset0 = vec3(5.0f, -rad0/2, -rad0/2);
vec3 sphereOffset1 = vec3(5.0f, -3.0f, -3.0f);
vec3 sphereOffset2 = vec3(5.0f, -7.5f, -9.0f);
vec3 sphereOffset3 = vec3(5.0f, -9.5f, -15.0f);
vec3 sphereOffset4 = vec3(5.0f, -13.5f, -17.0f);

vec3 Cloth::getSphereOffset0(){return sphereOffset0;}
vec3 Cloth::getSphereOffset1(){return sphereOffset1;}
vec3 Cloth::getSphereOffset2(){return sphereOffset2;}
vec3 Cloth::getSphereOffset3(){return sphereOffset3;}
vec3 Cloth::getSphereOffset4(){return sphereOffset4;}

Particle* Cloth::getParticle(int x, int y) {return &particles[y*num_particles_width + x];}
void Cloth::makeConstraint(Particle *p1, Particle *p2) {
    constraints.push_back(Constraint(p1,p2));
}


/* A private method used by drawShaded() and addWindForcesForTriangle() to retrieve the
   normal vector of the triangle defined by the position of the particles p1, p2, and p3.
   The magnitude of the normal vector is equal to the area of the parallelogram defined by p1, p2 and p3
   */
vec3 Cloth::calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3)
{
    vec3 pos1 = p1->getPos();
    vec3 pos2 = p2->getPos();
    vec3 pos3 = p3->getPos();

    vec3 v1 = pos2-pos1;
    vec3 v2 = pos3-pos1;

    return cross(v1,v2);
}

/* A private method used by windForce() to calcualte the wind force for a single triangle
   defined by p1,p2,p3*/
void Cloth::addWindForcesForTriangle(Particle *p1,Particle *p2,Particle *p3, const vec3 direction)
{
    vec3 normal = calcTriangleNormal(p1,p2,p3);
    vec3 d = normalize(normal);
    vec3 force = normal*(dot(d,direction));

    p1->addForce(force);
    p2->addForce(force);
    p3->addForce(force);
}


/* This is a important constructor for the entire system of particles and constraints*/
Cloth::Cloth(float width, float height, int num_particles_width, int num_particles_height, vec3 modelPosition, vec3 offset) :
    Entity(
            //vec3(0.0f,0.0f,-5.0f),
            modelPosition, //+ vec3(5.0f, 0.0f, 5.0f),
            vec3(0.0f,0.0f,0.0f)
          ),
    num_particles_width(num_particles_width),
    num_particles_height(num_particles_height) {
        particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

        // creating particles in a grid of particles from (0,0,0) to (width,-height,0)
        for(int x=0; x<num_particles_width; x++){
            for(int y=0; y<num_particles_height; y++){
                vec3 pos = offset + vec3(width * (x/(float)num_particles_width),
                        0,
                        -height * (y/(float)num_particles_height));
                particles[y*num_particles_width+x]= Particle(pos); // insert particle in column x at y'th row
                //@TODO wird das hier optimal gefüllt? Sollte eher x*num_height+y sein damit die bytes sequentiell und nicht mit sprüngen geschrieben werden.
            }
        }

        // Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
        for(int x=0; x<num_particles_width; x++){
            for(int y=0; y<num_particles_height; y++){
                if (x<num_particles_width-1)
                    makeConstraint(getParticle(x,y),getParticle(x+1,y));
                if (y<num_particles_height-1)
                    makeConstraint(getParticle(x,y),getParticle(x,y+1));
                if (x<num_particles_width-1 && y<num_particles_height-1)
                    makeConstraint(getParticle(x,y),getParticle(x+1,y+1));
                if (x<num_particles_width-1 && y<num_particles_height-1)
                    makeConstraint(getParticle(x+1,y),getParticle(x,y+1));
            }
        }


        // Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
        for(int x=0; x<num_particles_width; x++){
            for(int y=0; y<num_particles_height; y++){
                if (x<num_particles_width-2)
                    makeConstraint(getParticle(x,y),getParticle(x+2,y));
                if (y<num_particles_height-2)
                    makeConstraint(getParticle(x,y),getParticle(x,y+2));
                if (x<num_particles_width-2 && y<num_particles_height-2)
                    makeConstraint(getParticle(x,y),getParticle(x+2,y+2));
                if (x<num_particles_width-2 && y<num_particles_height-2)
                    makeConstraint(getParticle(x+2,y),getParticle(x,y+2));
            }
        }

        // making the upper left most three and right most three particles unmovable
        for(int i=0;i<3; i++){
            getParticle(0+i ,0)->offsetPos(vec3(2,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
            getParticle(0+i ,0)->makeUnmovable();

            getParticle(0+i ,0)->offsetPos(vec3(-2,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
            getParticle(num_particles_width-1-i ,0)->makeUnmovable();

        }

        //Create ACGL::ArrayBuffer
        std::vector<Vertex> vertexData;
        for(int x = 0; x<num_particles_width-1; x++){
            for(int y=0; y<num_particles_height-1; y++){
                //TODO write in OpenGL buffers
                //drawTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
                //drawTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
                vec3 uv = vec3(0);
                if (x%2) // red and white color is interleaved according to which column number
                    uv = vec3(0.6f,0.2f,0.2f);
                else
                    uv = vec3(1.0f,1.0f,1.0f);

                insertTriangle(getParticle(x, y), getParticle(x+1, y), getParticle(x, y+1), uv, vertexData);
                insertTriangle(getParticle(x+1, y), getParticle(x, y+1), getParticle(x+1, y+1), uv, vertexData);
            }
        }
        ab = SharedArrayBuffer(new ArrayBuffer());
        ab->defineAttribute("aPosition", GL_FLOAT, 3);
        ab->defineAttribute("aColor", GL_FLOAT, 3);
        ab->defineAttribute("aNormal", GL_FLOAT, 3);
        ab->setDataElements(vertexData.size(), value_ptr(vertexData[0].position), GL_DYNAMIC_DRAW);

        vao = SharedVertexArrayObject(new VertexArrayObject());
        vao->attachAllAttributes(ab);
    }


/* drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
   Called from the display() method
   The cloth is seen as consisting of triangles for four particles in the grid as follows:

   (x,y)   *--* (x+1,y)
   | /|
   |/ |
   (x,y+1) *--* (x+1,y+1)

*/
void Cloth::render(ACGL::OpenGL::SharedShaderProgram shader, mat4 *viewProjectionMatrix){
    // reset normals (which where written to last frame)
    std::vector<Particle>::iterator particle;
    for(particle = particles.begin(); particle != particles.end(); particle++){
        (*particle).resetNormal();
    }

    //create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
    for(int x = 0; x<num_particles_width-1; x++){
        for(int y=0; y<num_particles_height-1; y++){
            vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
            getParticle(x+1,y)->addToNormal(normal);
            getParticle(x,y)->addToNormal(normal);
            getParticle(x,y+1)->addToNormal(normal);

            normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
            getParticle(x+1,y+1)->addToNormal(normal);
            getParticle(x+1,y)->addToNormal(normal);
            getParticle(x,y+1)->addToNormal(normal);
        }
    }

    std::vector<Vertex> vertexData;
    for(int x = 0; x<num_particles_width-1; x++){
        vec3 uv = vec3(0);
        if ((x/2)%2) // red and white color is interleaved according to which column number
            uv = vec3(0.6f,0.2f,0.2f);
        else
            uv = vec3(1.0f,1.0f,1.0f);

        for(int y=0; y<num_particles_height-1; y++){
            //TODO write in OpenGL buffers
            insertTriangle(getParticle(x, y), getParticle(x+1, y), getParticle(x, y+1), uv, vertexData);
            insertTriangle(getParticle(x+1, y), getParticle(x, y+1), getParticle(x+1, y+1), uv, vertexData);

        }
    }

    // Give our vertices to OpenGL.


    //glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), value_ptr(vertexData[0].position), GL_STREAM_DRAW);
    ab->setSubData(0, vertexData.size() * sizeof(Vertex), value_ptr(vertexData[0].position));



    mat4 modelMatrix = translate(getPosition()) * getRotation() *
        scale<float>(vec3(0.4f));
    shader->setUniform("uModelMatrix", modelMatrix);

    mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
    shader->setUniform("uMVP",         mvp);



    // Draw the triangle !
    vao->render(); // Starting from vertex 0; 3 vertices -> 1 triangle


}

/* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
void Cloth::insertTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 uv, std::vector<Vertex> &vertexData) {
    Vertex v = {p1->getPos(), uv, p1->getNormal()};
    vertexData.push_back(v);
    v = {p2->getPos(), uv, p2->getNormal()};
    vertexData.push_back(v);
    v = {p3->getPos(), uv, p3->getNormal()};
    vertexData.push_back(v);
}

/* this is an important methods where the time is progressed one time step for the entire cloth.
   This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
   */
void Cloth::timeStep(float dt){
    std::vector<Constraint>::iterator constraint;
    for(int i=0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times{
        for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
        {
            (*constraint).statisfyConstraint(); // satisfy constraint.
        }

    std::vector<Particle>::iterator particle;
    for(particle = particles.begin(); particle != particles.end(); particle++){
        (*particle).timeStep(dt); // calculate the position of each particle at the next time step.
    }
}
/* used to add gravity (or any other arbitrary vector) to all particles*/
void Cloth::addForce(const vec3 direction){
    std::vector<Particle>::iterator particle;
    for(particle = particles.begin(); particle != particles.end(); particle++)
    {
        (*particle).addForce(direction); // add the forces to each particle
    }

}

/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
void Cloth::windForce(const vec3 direction){
    for(int x = 0; x<num_particles_width-1; x++)
    {
        for(int y=0; y<num_particles_height-1; y++)
        {
            addWindForcesForTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),direction);
            addWindForcesForTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),direction);
        }
    }
}

/* used to detect and resolve the collision of the cloth with the ball.
   This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
   This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
   */
void Cloth::modelCollision(){
    std::vector<Particle>::iterator particle;

    for(particle = particles.begin(); particle != particles.end(); particle++)
    {
        vec3 v0 = (*particle).getPos()-(sphereOffset0);
        float l0 = length(v0);
        if (l0 < rad0) // if the particle is inside the ball
        {
            (*particle).offsetPos(normalize(v0)*(rad0-l0)); // project the particle to the surface of the ball
        }

        //vec3 v1 = (*particle).getPos()-(modelPosition + sphereOffset1);
        //float l1 = length(v1);
        //if ( length(v1) < rad1) // if the particle is inside the ball
        //{
        //    //debug() << "collision detected @ "<< &particle << endl;
        //    (*particle).offsetPos(normalize(v1)*(rad1-l1)); // project the particle to the surface of the ball
        //}

        //vec3 v2 = (*particle).getPos()-(modelPosition + sphereOffset2);
        //float l2 = length(v2);
        //if ( length(v2) < rad2) // if the particle is inside the ball
        //{
        //    //debug() << "collision detected @ "<< &particle << endl;
        //    (*particle).offsetPos(normalize(v2)*(rad2-l2)); // project the particle to the surface of the ball
        //}

        //vec3 v3 = (*particle).getPos()-(modelPosition + sphereOffset3);
        //float l3 = length(v3);
        //if ( length(v3) < rad3) // if the particle is inside the ball
        //{
        //    //debug() << "collision detected @ "<< &particle << endl;
        //    (*particle).offsetPos(normalize(v3)*(rad3-l3)); // project the particle to the surface of the ball
        //}

        //vec3 v4 = (*particle).getPos()-(modelPosition + sphereOffset4);
        //float l4 = length(v4);
        //if ( length(v4) < rad4) // if the particle is inside the ball
        //{
        //    //debug() << "collision detected @ "<< &particle << endl;
        //    (*particle).offsetPos(normalize(v4)*(rad4-l4)); // project the particle to the surface of the ball
        //}
    }
}
