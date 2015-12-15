#include "Model.hh"


using namespace std;
using namespace ACGL;
using namespace ACGL::OpenGL;

Model::Model(string fileName, float scale) : scale(scale) {
  this->object = OpenGL::VertexArrayObjectCreator(fileName).create();
}

Model::Model() {}

Model::~Model() {}

void Model::setScale(float scale) {
  this->scale = scale;
}

void Model::render() {
  object->render();
}
