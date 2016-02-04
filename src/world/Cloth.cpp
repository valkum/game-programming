#include "world/Cloth.hh"
#include <ACGL/OpenGL/GL.hh>
#include <ACGL/Utils/Log.hh>


#define CONSTRAINT_ITERATIONS 5

using namespace std;
using namespace glm;
using namespace ACGL::OpenGL;
using namespace ACGL::Utils;
class Cloth;

float rad0  = 1.7f;
float rad1  = 2.6f;
float rad2  = 2.6f;
float rad3  = 1.6f;
float rad4  = 1.6f;
float rad5  = 2.6f;
float rad6  = 2.0f;
float rad7  = 2.0f;
float rad8  = 1.6f;
float rad9  = 1.6f;
float rad10 = 2.6f;
float rad11 = 1.6f;
float rad12 = 1.6f;
float rad13 = 1.0f;
float rad14 = 1.0f;
float rad15 = 1.6f;
float rad16 = 1.6f;
float rad17 = 1.8f;
float rad18 = 1.8f;


//Head
vec3 sphereOffset0 = vec3(0.0f, 4.0f, -6.0f);
//Shoulder
vec3 sphereOffset1 = vec3(-1.1f, 2.0f, -3.0f);
vec3 sphereOffset2 = vec3( 1.1f, 2.0f, -3.0f);
vec3 sphereOffset3 = vec3(-2.0f, 1.3f, -3.0f);
vec3 sphereOffset4 = vec3( 2.0f, 1.3f, -3.0f);
//upper Torso
vec3 sphereOffset5 = vec3(0.0f, 1.5f, -1.0f);
vec3 sphereOffset6 = vec3(-1.0f, 1.8f, 0.2f);
vec3 sphereOffset7 = vec3( 1.0f, 1.8f, 0.2f);
vec3 sphereOffset8 = vec3(-1.0f, 0.8f, 1.8f);
vec3 sphereOffset9 = vec3( 1.0f, 0.8f, 1.8f);
//lower Torso
vec3 sphereOffset10 = vec3(0.0f, -1.5f, 5.0f);
vec3 sphereOffset11 = vec3( 1.0f, 0.0f, 4.0f);
vec3 sphereOffset12 = vec3(-1.0f, 0.0f, 4.0f);
//Ass
vec3 sphereOffset13 = vec3(-1.0f, 0.0f, 7.8f);
vec3 sphereOffset14 = vec3( 1.0f, 0.0f, 7.8f);
vec3 sphereOffset15 = vec3(-1.5f, -1.0f, 7.9f);
vec3 sphereOffset16 = vec3( 1.5f, -1.0f, 7.9f);
//Legs
vec3 sphereOffset17 = vec3(-1.5f, -2.8f, 10.0f);
vec3 sphereOffset18 = vec3( 1.5f, -2.8f, 10.0f);



vec3 Cloth::getSphereOffset0 (){return sphereOffset0;}
vec3 Cloth::getSphereOffset1 (){return sphereOffset1;}
vec3 Cloth::getSphereOffset2 (){return sphereOffset2;}
vec3 Cloth::getSphereOffset3 (){return sphereOffset3;}
vec3 Cloth::getSphereOffset4 (){return sphereOffset4;}
vec3 Cloth::getSphereOffset5 (){return sphereOffset5;}
vec3 Cloth::getSphereOffset6 (){return sphereOffset6;}
vec3 Cloth::getSphereOffset7 (){return sphereOffset7;}
vec3 Cloth::getSphereOffset8 (){return sphereOffset8;}
vec3 Cloth::getSphereOffset9 (){return sphereOffset9;}
vec3 Cloth::getSphereOffset10(){return sphereOffset10;}
vec3 Cloth::getSphereOffset11(){return sphereOffset11;}
vec3 Cloth::getSphereOffset12(){return sphereOffset12;}
vec3 Cloth::getSphereOffset13(){return sphereOffset13;}
vec3 Cloth::getSphereOffset14(){return sphereOffset14;}
vec3 Cloth::getSphereOffset15(){return sphereOffset15;}
vec3 Cloth::getSphereOffset16(){return sphereOffset16;}
vec3 Cloth::getSphereOffset17(){return sphereOffset17;}
vec3 Cloth::getSphereOffset18(){return sphereOffset18;}



float Cloth::getSphereRadius0(){return rad0;}
float Cloth::getSphereRadius1(){return rad1;}
float Cloth::getSphereRadius2(){return rad2;}
float Cloth::getSphereRadius3(){return rad3;}
float Cloth::getSphereRadius4(){return rad4;}
float Cloth::getSphereRadius5(){return rad5;}
float Cloth::getSphereRadius6(){return rad6;}
float Cloth::getSphereRadius7(){return rad7;}
float Cloth::getSphereRadius8(){return rad8;}
float Cloth::getSphereRadius9(){return rad9;}
float Cloth::getSphereRadius10(){return rad10;}
float Cloth::getSphereRadius11(){return rad11;}
float Cloth::getSphereRadius12(){return rad12;}
float Cloth::getSphereRadius13(){return rad13;}
float Cloth::getSphereRadius14(){return rad14;}
float Cloth::getSphereRadius15(){return rad15;}
float Cloth::getSphereRadius16(){return rad16;}
float Cloth::getSphereRadius17(){return rad17;}
float Cloth::getSphereRadius18(){return rad18;}

Particle* Cloth::getParticle(int x, int y) {return &particles[y*num_particles_width + x];}
void Cloth::makeConstraint(Particle *p1, Particle *p2) {
  constraints.push_back(Constraint(p1,p2));
}

void Cloth::scaleCollisionModel(float scalar){
  //Head
  sphereOffset0 = scalar * sphereOffset0;
  rad0  = scalar * rad0;  
  //Shoulder
  sphereOffset1 = scalar * sphereOffset1;
  sphereOffset2 = scalar * sphereOffset2;
  sphereOffset3 = scalar * sphereOffset3;
  sphereOffset4 = scalar * sphereOffset4;
  rad1  = scalar * rad1;  
  rad2  = scalar * rad2;  
  rad3  = scalar * rad3;  
  rad4  = scalar * rad4;  
  //upper Torso
  sphereOffset5 = scalar * sphereOffset5;
  sphereOffset6 = scalar * sphereOffset6;
  sphereOffset7 = scalar * sphereOffset7;
  sphereOffset8 = scalar * sphereOffset8;
  sphereOffset9 = scalar * sphereOffset9;
  rad5  = scalar * rad5;  
  rad6  = scalar * rad6;  
  rad7  = scalar * rad7;  
  rad8  = scalar * rad8;  
  rad9  = scalar * rad9;  
  //lower Torso
  sphereOffset10 = scalar * sphereOffset10;
  sphereOffset11 = scalar * sphereOffset11;
  sphereOffset12 = scalar * sphereOffset12;
  rad10 = scalar * rad10;  
  rad11 = scalar * rad11;  
  rad12 = scalar * rad12;  
  //Ass
  sphereOffset13 = scalar * sphereOffset13;
  sphereOffset14 = scalar * sphereOffset14;
  sphereOffset15 = scalar * sphereOffset15;
  sphereOffset16 = scalar * sphereOffset16;
  rad13 = scalar * rad13;  
  rad14 = scalar * rad14;  
  rad15 = scalar * rad15;  
  rad16 = scalar * rad16;  
  //Legs
  sphereOffset17 = scalar * sphereOffset17;
  sphereOffset18 = scalar * sphereOffset18;
  rad17 = scalar * rad17;  
  rad18 = scalar * rad18;  
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
Cloth::Cloth(float width, float height, int num_particles_width, int num_particles_height, vec3 modelPosition, vec3 offset, float scalar) :
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
        vec3 pos = offset + vec3((-width) * (x/(float)num_particles_width),
            0,
            height * (y/(float)num_particles_height));
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
      //getParticle(0+i ,0)->offsetPos(vec3(-1.5f, -0.5f, -1.5f)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
      getParticle(0+i ,0)->offsetPos(vec3(-0.3f, -0.5f, -1.5f)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
      getParticle(0+i ,0)->makeUnmovable();

      //getParticle(num_particles_width-1-i ,0)->offsetPos(vec3(1.5f, -0.5f, -1.5f)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
      getParticle(num_particles_width-1-i ,0)->offsetPos(vec3(0.3f, -0.5f, -1.5f)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
      getParticle(num_particles_width-1-i ,0)->makeUnmovable();

    }

    //scale the collision model spheres and radiuses
    //scaleCollisionModel(scalar); 

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
void Cloth::render(ACGL::OpenGL::SharedShaderProgram shader, mat4 *viewProjectionMatrix, float scalar){
  // reset normals (which where written to last frame)
  std::vector<Particle>::iterator particle;
  for(particle = particles.begin(); particle != particles.end(); particle++){
    (*particle).resetNormal();
  }

  //create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
  for(int x = 0; x<num_particles_width-1; x++){
    for(int y=0; y<num_particles_height-1; y++){
      vec3 normal = calcTriangleNormal(getParticle(x,y),getParticle(x+1,y),getParticle(x,y+1));
      getParticle(x+1,y)->addToNormal(normal);
      getParticle(x,y)->addToNormal(normal);
      getParticle(x,y+1)->addToNormal(normal);

      normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x+1,y+1),getParticle(x,y+1));
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
    scale<float>(vec3(scalar * 0.4f));
  shader->setUniform("uModelMatrix", modelMatrix);

  mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
  shader->setUniform("uMVP",         mvp);



  // Draw the triangle !
  vao->render(); // Starting from vertex 0; 3 vertices -> 1 triangle


}

/* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
void Cloth::insertTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 uv, std::vector<Vertex> &vertexData) {
  Vertex v = {p1->getPos(), p1->getNormal()};
  vertexData.push_back(v);
  v = {p2->getPos(), p2->getNormal()};
  vertexData.push_back(v);
  v = {p3->getPos(), p3->getNormal()};
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

    vec3 v1 = (*particle).getPos()-(sphereOffset1);
    float l1 = length(v1);
    if (l1 < rad1) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v1)*(rad1-l1)); // project the particle to the surface of the ball
    }

    vec3 v2 = (*particle).getPos()-(sphereOffset2);
    float l2 = length(v2);
    if (l2 < rad2) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v2)*(rad2-l2)); // project the particle to the surface of the ball
    }

    vec3 v3 = (*particle).getPos()-(sphereOffset3);
    float l3 = length(v3);
    if (l3 < rad3) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v3)*(rad3-l3)); // project the particle to the surface of the ball
    }

    vec3 v4 = (*particle).getPos()-(sphereOffset4);
    float l4 = length(v4);
    if (l4 < rad4) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v4)*(rad4-l4)); // project the particle to the surface of the ball
    }

    vec3 v5 = (*particle).getPos()-(sphereOffset5);
    float l5 = length(v5);
    if (l5 < rad5) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v5)*(rad5-l5)); // project the particle to the surface of the ball
    }

    vec3 v6 = (*particle).getPos()-(sphereOffset6);
    float l6 = length(v6);
    if (l6 < rad6) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v6)*(rad6-l6)); // project the particle to the surface of the ball
    }

    vec3 v7 = (*particle).getPos()-(sphereOffset7);
    float l7 = length(v7);
    if (l7 < rad7) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v7)*(rad7-l7)); // project the particle to the surface of the ball
    }

    vec3 v8 = (*particle).getPos()-(sphereOffset8);
    float l8 = length(v8);
    if (l8 < rad8) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v8)*(rad8-l8)); // project the particle to the surface of the ball
    }

    vec3 v9 = (*particle).getPos()-(sphereOffset9);
    float l9 = length(v9);
    if (l9 < rad9) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v9)*(rad9-l9)); // project the particle to the surface of the ball
    }

    vec3 v10 = (*particle).getPos()-(sphereOffset10);
    float l10 = length(v10);
    if (l10 < rad10) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v10)*(rad10-l10)); // project the particle to the surface of the ball
    }

    vec3 v11 = (*particle).getPos()-(sphereOffset11);
    float l11 = length(v11);
    if (l11 < rad11) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v11)*(rad11-l11)); // project the particle to the surface of the ball
    }

    vec3 v12 = (*particle).getPos()-(sphereOffset12);
    float l12 = length(v12);
    if (l12 < rad12) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v12)*(rad12-l12)); // project the particle to the surface of the ball
    }

    vec3 v13 = (*particle).getPos()-(sphereOffset13);
    float l13 = length(v13);
    if (l13 < rad13) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v13)*(rad13-l13)); // project the particle to the surface of the ball
    }

    vec3 v14 = (*particle).getPos()-(sphereOffset14);
    float l14 = length(v14);
    if (l14 < rad14) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v14)*(rad14-l14)); // project the particle to the surface of the ball
    }

    vec3 v15 = (*particle).getPos()-(sphereOffset15);
    float l15 = length(v15);
    if (l15 < rad15) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v15)*(rad15-l15)); // project the particle to the surface of the ball
    }

    vec3 v16 = (*particle).getPos()-(sphereOffset16);
    float l16 = length(v16);
    if (l16 < rad16) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v16)*(rad16-l16)); // project the particle to the surface of the ball
    }

    vec3 v17 = (*particle).getPos()-(sphereOffset17);
    float l17 = length(v17);
    if (l17 < rad17) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v17)*(rad17-l17)); // project the particle to the surface of the ball
    }

    vec3 v18 = (*particle).getPos()-(sphereOffset18);
    float l18 = length(v18);
    if (l18 < rad18) // if the particle is inside the ball
    {
      (*particle).offsetPos(normalize(v18)*(rad18-l18)); // project the particle to the surface of the ball
    }
  }
}
void  Cloth::moveAnchorPoints(vec3 v){
  for(int i=0;i<3; i++){
    getParticle(0+i ,0)->makeMovable();
    getParticle(0+i ,0)->offsetPos(v);
    getParticle(0+i ,0)->makeUnmovable();

    getParticle(num_particles_width-1-i ,0)->makeMovable();
    getParticle(num_particles_width-1-i ,0)->offsetPos(v);
    getParticle(num_particles_width-1-i ,0)->makeUnmovable();
  }
}
