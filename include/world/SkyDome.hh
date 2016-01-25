#ifndef SKYBOX_H
#define SKYBOX_H

#include "world/Entity.hh"
#include "Model.hh"
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/OpenGL/Objects.hh>

using namespace std;

class SkyDome : public Entity {
public:

  SkyDome(Model         model,
         string fileName);
  SkyDome();
  ~SkyDome();
  Model getModel() {return model;};
  void                               render(
    ACGL::OpenGL::SharedShaderProgram shader,
    mat4                             *viewProjectioMatrix);
  ACGL::OpenGL::SharedTexture2D getTexture(){return texture;};

private:

  Model model;
  ACGL::OpenGL::SharedTexture2D texture;
};

#endif // SKYBOX_H
