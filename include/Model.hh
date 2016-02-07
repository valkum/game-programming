#ifndef MODEL_H
#define MODEL_H

#include <ACGL/OpenGL/Creator/VertexArrayObjectCreator.hh>

class Model {
public:

  Model(std::string fileName,
        float       scale);
  Model(ACGL::OpenGL::SharedVertexArrayObject model,
        float       scale, int primCount);
  Model();
  ~Model();
  inline float getScale() {
    return scale;
  }
  inline int getPrimCount() {return primCount;};
  void setScale(float scale);
  void render();

  ACGL::OpenGL::SharedVertexArrayObject getVAO(){ return object; }

private:

  ACGL::OpenGL::SharedVertexArrayObject object;
  float scale;
  GLsizei primCount;
};


#endif // MODEL_H
