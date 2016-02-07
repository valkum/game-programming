#include "Model.hh"
#include <ACGL/Utils/Log.hh>


using namespace std;
using namespace ACGL;
using namespace ACGL::OpenGL;

Model::Model(string fileName, float scale) : scale(scale) {
  this->object = VertexArrayObjectCreator(fileName).create();
}
Model::Model(SharedVertexArrayObject model, float scale) : scale(scale) {
  this->object = model;
}

Model::Model() {}

Model::~Model() {}

void Model::setScale(float scale) {
  this->scale = scale;
}

void Model::render() {
  object->render();
}
