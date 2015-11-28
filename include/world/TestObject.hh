#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include "world/Entity.hh"
#include "Model.hh"
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/OpenGL/Creator/Texture2DCreator.hh>
#include <ACGL/OpenGL/Data/TextureLoadStore.hh>

class TestObject : public Entity {
public:

  TestObject(Model model,
             vec3  position,
             vec3  rotation);
  TestObject();
  ~TestObject();
  void                                 Render(
    ACGL::OpenGL::SharedShaderProgram shader,
    mat4                             *viewProjectioMatrix);
  inline ACGL::OpenGL::SharedTexture2D getTexture() {
    return this->texture;
  }

private:

  Model model;
  ACGL::OpenGL::SharedTexture2D texture;
};

#endif // ifndef TESTOBJECT_H
