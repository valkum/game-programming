#include "world/entity.h"

class Entity;

using namespace glm;

Entity::Entity(vec3 position, vec3 rotation) {
    this->position = position;
    setRotation(rotation);
}

Entity::Entity(vec3 position, mat4 rotation) {
    this->position = position;
    this->rotation = rotation;
}
Entity::Entity() {
    
}
Entity::~Entity() {
}

void Entity::setRotation(vec3 rotation) {
    this->rotation = glm::rotate(rotation.x, vec3(1.0f, 0.0f, 0.0f)) * 
        glm::rotate(rotation.y, vec3(0.0f, 1.0f, 0.0f)) * 
        glm::rotate(rotation.z, vec3(0.0f, 0.0f, 1.0f));
}