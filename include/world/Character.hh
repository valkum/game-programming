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
            vec3  rotation,
            float scalar);
  Character();
  ~Character();
  void render(mat4 *viewProjectioMatrix);

  SharedVertexArrayObject getVAO() {
    return model.getVAO(); 
  }

  //inline vec3 getWindDirection(){
  //  return windDirection;
  //}

  //inline void setWindDirection(vec3 windUpdate){
  //  windDirection = windUpdate;
  //}
  
  Cloth * getCloth(){
    return cloth;
  }

  void setCharacterPosition(vec3 position);

  void draw(mat4 viewMatrix);

  void rotateZ(float angle);

  void update(float dt);

private:
  Model model;
  SharedShaderProgram clothShader;
  SharedShaderProgram lowPolyManShader;
  Cloth *cloth;
  float scalar;
  float angle = 0.0f;
  vec3 clothOffset = vec3(4.0f, 4.0f, -3.0f);
  vec3 rotation = vec3(0.0f, 0.0f, 0.0f);
  vec3 gravity = vec3(0.0f, -9.81f, 0.0f);
};
