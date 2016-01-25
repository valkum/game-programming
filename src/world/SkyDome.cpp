#include "world/SkyDome.hh"

#include "ACGL/OpenGL/Creator/Texture2DCreator.hh"
#include <ACGL/Utils/Log.hh>

using namespace glm;
using namespace std;
using namespace ACGL;
using namespace ACGL::OpenGL;
using namespace ACGL::Utils;

class SkyDome;

SkyDome::SkyDome(Model         model,
               string fileName) : Entity(vec3(0.0f, 0.0f, 0.0f),
                                                 vec3(0.0f, 0.0f, 0.0f)), model(model) {
  texture = Texture2DFileManager::the()->get(Texture2DCreator(fileName));
  texture->generateMipmaps();
  texture->setMinFilter(GL_NEAREST_MIPMAP_LINEAR);
  texture->setMagFilter(GL_LINEAR);
}

SkyDome::SkyDome() {}

SkyDome::~SkyDome() {}

void SkyDome::render(ACGL::OpenGL::SharedShaderProgram shader,
                    mat4                             *viewProjectionMatrix) {
  mat4 modelMatrix = translate(getPosition()) * getRotation() *
                     scale<float>(vec3(model.getScale()));

  shader->setUniform("uModelMatrix",          modelMatrix);
  shader->setUniform("uViewProjectionMatrix", *viewProjectionMatrix);

  mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
  shader->setUniform("uMVP",                  mvp);
  model.render();
}
