#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include "world/Entity.hh"
#include "Model.hh"
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/OpenGL/Creator/Texture2DCreator.hh>
#include <ACGL/OpenGL/Data/TextureLoadStore.hh>

class Object : public Entity {
public:

  Object(Model* model,
             vec3  position,
             vec3  rotation);
  Object();
  ~Object();
  void                                 render(
    ACGL::OpenGL::SharedShaderProgram shader,
    mat4                             *viewProjectioMatrix);
  inline ACGL::OpenGL::SharedTexture2D getTexture() {
    return this->texture;
  }
  inline void setTexture(ACGL::OpenGL::SharedTexture2D texture) {
    this->texture = texture;
  }

  inline Model* getModel(){
    return this->model;
  }

  ACGL::OpenGL::SharedVertexArrayObject getVAO() { return model->getVAO(); }

private:
  Model* model;
  ACGL::OpenGL::SharedTexture2D texture;
};

#endif // ifndef TESTOBJECT_H
