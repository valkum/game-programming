#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Particle.hh"
#include "Constraint.hh"
#include "world/Entity.hh"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ACGL/OpenGL/Objects.hh>

#ifndef GLINCLUDED
#define GLINCLUDED
# include <ACGL/OpenGL/GL.hh>
# include <GLFW/glfw3.h>
#endif

using namespace glm;

struct Vertex {
  vec3 position;
  vec3 normal;
};

class Cloth {
  private:
    int num_particles_width;
    int num_particles_height;
    Entity *character;

    ACGL::OpenGL::SharedVertexArrayObject vao;
    ACGL::OpenGL::SharedArrayBuffer ab;

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;

    Particle* getParticle(int x, int y);
    void makeConstraint(Particle *p1, Particle *p2);

    void scaleCollisionModel(float scalar);

    void insertTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 uv, std::vector<Vertex> &vertexData);

    vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3);

    void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const vec3 direction);

    void drawTriangle(Particle *p1, Particle *p2, Particle *p3);
    
  public:
    Cloth(float width, float height, int num_particles_width, int num_particles_height, vec3 offset, Entity *character);

    vec3 getSphereOffset0();
    vec3 getSphereOffset1();
    vec3 getSphereOffset2();
    vec3 getSphereOffset3();
    vec3 getSphereOffset4();
    vec3 getSphereOffset5();
    vec3 getSphereOffset6();
    vec3 getSphereOffset7();
    vec3 getSphereOffset8();
    vec3 getSphereOffset9();
    vec3 getSphereOffset10();
    vec3 getSphereOffset11();
    vec3 getSphereOffset12();
    vec3 getSphereOffset13();
    vec3 getSphereOffset14();
    vec3 getSphereOffset15();
    vec3 getSphereOffset16();
    vec3 getSphereOffset17();
    vec3 getSphereOffset18();

    float getSphereRadius0();
    float getSphereRadius1();
    float getSphereRadius2();
    float getSphereRadius3();
    float getSphereRadius4();
    float getSphereRadius5();
    float getSphereRadius6();
    float getSphereRadius7();
    float getSphereRadius8();
    float getSphereRadius9();
    float getSphereRadius10();
    float getSphereRadius11();
    float getSphereRadius12();
    float getSphereRadius13();
    float getSphereRadius14();
    float getSphereRadius15();
    float getSphereRadius16();
    float getSphereRadius17();
    float getSphereRadius18();

    void render(ACGL::OpenGL::SharedShaderProgram shader, mat4 *viewProjectionMatrix);

    void timeStep(float dt);

    void addForce(const vec3 direction);

    void windForce(const vec3 direction);

    void modelCollision();

    void moveAnchorPoints(vec3 v);

    void doFrame();
    ACGL::OpenGL::SharedVertexArrayObject getVAO() { return vao; }
};

#endif
