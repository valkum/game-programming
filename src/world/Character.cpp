#include "world/Character.hh"

#include <ACGL/Utils/Log.hh>

using namespace ACGL::Utils;
using namespace std;

class Character;

Character::Character(vec3 position, vec3 rotation) : Entity(
    position,
    rotation) {
  this->model = model;
  cloth = new Cloth(8,15,24,24, position, clothOffset);
}

Character::Character() {}

Character::~Character() {}


void Character::render(ACGL::OpenGL::SharedShaderProgram shader,
                        mat4                             *viewProjectioMatrix) {
  mat4 modelMatrix = translate(getPosition()) * getRotation() *
                     scale<float>(vec3(model.getScale()));

  shader->setUniform("uModelMatrix", modelMatrix);

  mat4 mvp = (*viewProjectioMatrix) * modelMatrix;
  shader->setUniform("uMVP", mvp);
  model.render();
  cloth.render();
}

void Character::setCharacterPosition(vec3 position){
  this->setPosition(position);
  cloth->setPosition(position);
  model->setPosition(position + clothOffset);
}
