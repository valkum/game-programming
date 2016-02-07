#include "Model.hh"
#include <ACGL/Utils/Log.hh>


using namespace std;
using namespace ACGL;
using namespace ACGL::OpenGL;

Model::Model(string fileName, float scale) : scale(scale) {
  this->object = VertexArrayObjectCreator(fileName).create();
  this->primCount = 1;
}
Model::Model(SharedVertexArrayObject model, float scale, GLsizei primCount) : scale(scale) {
  this->object = model;
  this->primCount = primCount;
  Utils::debug()<<primCount<<std::endl;
}

Model::Model() {}

Model::~Model() {}

void Model::setScale(float scale) {
  this->scale = scale;
}

void Model::render() {
  object->bind();
  object->draw(primCount);
}
