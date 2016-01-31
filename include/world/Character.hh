#pragma once

#include "world/Entity.hh"
#include "Model.hh"
#include "Cloth.hh"
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/OpenGL/Creator/VertexArrayObjectCreator.hh>
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>

using namespace ACGL::OpenGL;

class Character : public Entity {
public:

  Character(vec3  position,
            vec3  rotation);
  Character();
  ~Character();
  void render(mat4 *viewProjectioMatrix);

  SharedVertexArrayObject getVAO() {
    return model.getVAO(); 
  }

  vec3 getWindDirection(){
    return windDirection;
  }

  void setWindDirection(vec3 windUpdate){
    windDirection = windUpdate;
  }
  
  Cloth * getCloth(){
    return cloth;
  }

  void setCharacterPosition(vec3 position);

  void draw(mat4 viewMatrix);

private:
  Model model;
  SharedShaderProgram clothShader;
  SharedShaderProgram lowPolyManShader;
  Cloth *cloth;
  vec3 clothOffset = vec3(4.0f, 4.0f, -3.0f);
  vec3 windDirection;
};
