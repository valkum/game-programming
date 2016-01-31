#pragma once

#include "world/Entity.hh"
#include "Model.hh"
#include "Cloth.hh"
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/OpenGL/Objects.hh>

class Character : public Entity {
public:

  Character(Model model,
             vec3  position,
             vec3  rotation);
  Character();
  ~Character();
  void render(
    ACGL::OpenGL::SharedShaderProgram shader,
    mat4 *viewProjectioMatrix);

  ACGL::OpenGL::SharedVertexArrayObject getVAO() {
    return model.getVAO(); 
  }

  vec3 getWindDirection(){
    return windDirection;
  }

  void setWindDirection(vec3 windUpdate){
    windDirection = windUpdate;
  }
  
  Cloth getCloth(){
    return cloth;
  }

  void setCharacterPosition(vec3 position);

private:
  Model model;
  Cloth *cloth;
  vec3 clothOffset = vec3(4.0f, 4.0f, -3.0f);
  vec3 windDirection;
};
