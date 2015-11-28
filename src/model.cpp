#include "Model.hh"


using namespace std;
using namespace ACGL;
using namespace ACGL::OpenGL;

Model::Model(string fileName, float scale) {
  this->scale  = scale;
  this->object = OpenGL::VertexArrayObjectCreator(fileName).create();
}

Model::Model() {}

Model::~Model() {}

void Model::setScale(float scale) {
  this->scale = scale;
}

void Model::Render() {
  object->render();
}
