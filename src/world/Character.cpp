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

  // clothShader = ShaderProgramCreator("cloth").attributeLocations(
  //     cloth->getVAO()->getAttributeLocations()).create();

  // lowPolyManShader = ShaderProgramCreator("low_poly_man").attributeLocations(
  //     model.getVAO()->getAttributeLocations()).create();
}

Character::Character() {}

Character::~Character() {}



void Character::render(SharedShaderProgram shader, mat4 *viewProjectionMatrix) {
  mat4 modelMatrix = translate(getPosition()) * getRotation() *
                     scale<float>(vec3(model.getScale()));

  shader->setUniform("uModelMatrix", modelMatrix);

  mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
  shader->setUniform("uMVP", mvp);
  model.render();

  cloth->render(shader, viewProjectionMatrix, this->scalar);
}

void Character::setCharacterPosition(vec3 position){
  this->setPosition(position);
  cloth->setPosition(position);
}

void Character::rotateZ(float angle){
  //this->setRotation(vec3(0.0f,0.0f,angle));
  //cloth->setRotation(vec3(0.0f,0.0f,angle));
  this->angle = angle;
  gravity = glm::rotateZ(vec3(0.0f, -9.0f, 0.0f), -(this->angle));
  //int degree = (int (angle * 180 / M_PI) % 360);
  //debug() << "supposed angle: " << -degree << "\t rotated gravity: " << glm::to_string(gravity) << "\t length: " << glm::fastLength(gravity) << endl;

  //vec3 rotationAxis(0.0f, 0.0f, 1.0f);
  //this->setRotation(glm::rotate(angle, rotationAxis));
  this->setRotation(vec3(0.0f, M_PI, angle));
  //cloth->setRotation(glm::rotate(angle, rotationAxis));
  cloth->setRotation(vec3(0.0f, M_PI, angle));
}

void Character::update(float dt){
  cloth->addForce(dt*gravity); // add gravity each frame, pointing down
  cloth->windForce(vec3(0.0f, 0.05f,0.2f));
  cloth->timeStep(dt); // calculate the particle positions of the next frame
  cloth->modelCollision();
}
