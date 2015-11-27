#ifndef SKYBOX_H
#define SKYBOX_H

#include "world/entity.h"
#include "model.h"
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/OpenGL/Objects.hh>

using namespace std;

class Skybox : public Entity {
public:
    Skybox(Model model, vector<string> fileNames);
    Skybox();
    ~Skybox();
    void Render(ACGL::OpenGL::SharedShaderProgram shader, mat4* viewProjectioMatrix);
    ACGL::OpenGL::SharedTextureCubeMap getTexture();
private:
    Model model;
    ACGL::OpenGL::SharedTextureCubeMap texture;
};

#endif // SKYBOX_H
