#include "world/skybox.h"

#include "TextureLoadStore.h"
#include <ACGL/Utils/Log.hh>

using namespace glm;
using namespace std;
using namespace ACGL;
using namespace ACGL::OpenGL;
using namespace ACGL::Utils;

class Skybox;

Skybox::Skybox(Model model, vector<string> fileNames) : Entity(vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,0.0f)){
    this->model = model;
    SharedTextureCubeMap loadedTexture = loadTextureCubeMap(fileNames);
    debug()<<"Scale of Skybox: "<<model.getScale()<<endl;
    //@TODO: Add TextureCubeFileManager for File Singletons.
    //this->texture = TextureFileManager::the()->get(loadedTexture);
    this->texture = loadedTexture;

}
Skybox::Skybox(){

}
Skybox::~Skybox(){

}

void Skybox::Render(ACGL::OpenGL::SharedShaderProgram shader, mat4* viewProjectioMatrix) {
    mat4 modelMatrix = translate(getPosition()) * getRotation() * scale<float>(vec3(model.getScale()));
    shader->setUniform("uModelMatrix", modelMatrix);
    shader->setUniform("uViewProjectionMatrix", *viewProjectioMatrix);

    mat4 mvp = (*viewProjectioMatrix) * modelMatrix;
    shader->setUniform("uMVP", mvp);
    model.Render();
}

SharedTextureCubeMap Skybox::getTexture() {
    return texture;
}