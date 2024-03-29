#include "world/Object.hh"

#include <ACGL/Utils/Log.hh>

using namespace ACGL::Utils;
using namespace std;

class Object;

Object::Object(Model* model, vec3 position, vec3 rotation) : Entity(
    position,
    rotation) {
  this->model = model;

  // texture =
  // ACGL::OpenGL::loadTexture2D(ACGL::Base::Settings::the()->getFullTexturePath()
  // + "box.png");
  // texture = ACGL::OpenGL::Texture2DFileManager::the()->get(ACGL::OpenGL::Texture2DCreator("box.png"));

  // texture->generateMipmaps();
  // texture->setMinFilter(GL_NEAREST_MIPMAP_LINEAR);
  // texture->setMagFilter(GL_LINEAR);
  //debug() << "Scale of Object: " << model.getScale() << endl;
}

Object::Object() {}

Object::~Object() {}

void Object::render(ACGL::OpenGL::SharedShaderProgram shader,
                        mat4                             *viewProjectioMatrix) {
  mat4 modelMatrix = translate(getPosition()) * getRotation() *
                     scale<float>(vec3(model->getScale()));

  shader->setUniform("uModelMatrix", modelMatrix);

  mat4 mvp = (*viewProjectioMatrix) * modelMatrix;
  shader->setUniform("uMVP",         mvp);
  model->render();
}
