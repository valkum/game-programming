#include "world/Character.hh"

#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>
#include <ACGL/OpenGL/Data/GeometryDataLoadStore.hh>
#include <ACGL/Utils/Log.hh>
#include <ACGL/Base/Settings.hh>
#include "Helper.hh"

using namespace ACGL::Utils;
using namespace ACGL::Base;
using namespace std;

class Character;

Character::Character(vec3 position, vec3 rotation, float scalar) : Entity(
    position,
    rotation) {

  Settings::the()->setResourcePath(Helper::getExePath() + "/assets/");
  Settings::the()->setShaderPath("shaders/");
  Settings::the()->setGeometryPath("geometry/");

  //setup lowpolyman object
  this->rotation = rotation;
  this->scalar = scalar;
  this->model = Model("low_poly_man.obj", this->scalar);

  //setup cloth
  //cloth = new Cloth(8,15,24,24, position, scalar * clothOffset, this->scalar);
  cloth = new Cloth(8,15,24,24, position, clothOffset, this->scalar);
  cloth->setRotation(rotation);

  clothShader = ShaderProgramCreator("cloth").attributeLocations(
      cloth->getVAO()->getAttributeLocations()).create();

  lowPolyManShader = ShaderProgramCreator("low_poly_man").attributeLocations(
      model.getVAO()->getAttributeLocations()).create();
}

Character::Character() {}

Character::~Character() {}



void Character::render(mat4 *viewProjectionMatrix) {
  mat4 modelMatrix = translate(getPosition()) * getRotation() *
                     scale<float>(vec3(model.getScale()));

  lowPolyManShader->setUniform("uModelMatrix", modelMatrix);

  mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
  lowPolyManShader->setUniform("uMVP", mvp);
  model.render();
  cloth->render(clothShader, viewProjectionMatrix, this->scalar);
}

void Character::setCharacterPosition(vec3 position){
  this->setPosition(position);
  cloth->setPosition(position + clothOffset);
}

void Character::rotateZ(float angle){
}

void Character::update(){

}
