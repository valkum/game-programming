#ifndef MODEL_H
#define MODEL_H

#include <ACGL/OpenGL/Creator/VertexArrayObjectCreator.hh>

class Model {
public:

  Model(std::string fileName,
        float       scale);
  Model();
  ~Model();
  inline float getScale() {
    return scale;
  }

  void setScale(float scale);
  void render();

  ACGL::OpenGL::SharedVertexArrayObject getVAO(){ return object; }

private:

  ACGL::OpenGL::SharedVertexArrayObject object;
  float scale;
};


#endif // MODEL_H
