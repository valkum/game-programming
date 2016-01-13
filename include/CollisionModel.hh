#pragma once

#include <glm/glm.hpp>

#include "world/Entity.hh"
#include "Model.hh"
#include <ACGL/OpenGL/Managers.hh>
#include <ACGL/OpenGL/Objects.hh>

class CollisionModel : public Entity {
    public:

        CollisionModel(Model model,
                vec3 position,
                vec3 rotation);
        CollisionModel();
        ~CollisionModel();


    bool checkCollision(vec3 point){
    }

    private:
        Model model;
};
