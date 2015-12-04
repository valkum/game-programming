#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

class Entity {
public:

  Entity(vec3 position,
         vec3 rotation);
  Entity(vec3 position,
         mat4 rotation);
  Entity();
  ~Entity();
  inline vec3 getPosition() {
    return position;
  }

  inline mat4 getRotation() {
    return rotation;
  }

  inline void setPosition(vec3 position) {
    this->position = position;
  }

  void        setRotation(vec3 rotation);
  inline void setRotation(mat4 rotation) {
    this->rotation = rotation;
  }

protected:

  vec3 position;
  mat4 rotation;
};

#endif // ifndef ENTITY_H
