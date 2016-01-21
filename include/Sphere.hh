#pragma once
#include <vector>
#include "world/Entity.hh"
#include "world/Cloth.hh"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ACGL/OpenGL/Objects.hh>

#ifndef GLINCLUDED
#define GLINCLUDED
# include <ACGL/OpenGL/GL.hh>
# include <GLFW/glfw3.h>
#endif

using namespace glm;


class Sphere : public Entity {
  private:
    ACGL::OpenGL::SharedVertexArrayObject vao;
    ACGL::OpenGL::SharedArrayBuffer ab;

    std::vector<Vertex> vertexDataTest;

  public:
    Sphere(vec3 spherePosition):
      Entity(
          //vec3(0.0f,0.0f,-5.0f),
          spherePosition, //+ vec3(5.0f, 0.0f, 5.0f),
          vec3(0.0f,0.0f,0.0f)
          ){
        //static GLuint vertexArrayObject = 0;
        //static int elementCount;
        //static GLuint sphereTex;
        std::vector<Vertex> vertexData;
        int slices = 64;
        int stacks = 32;
        float radius = 1.9;
        //int vertexCount = (stacks+1) * (slices + 1);
        float piDivStacks = M_PI / stacks;
        float PIDiv2 = M_PI / 2;
        float PI2 = M_PI * 2;

        for (int j = 0; j <= stacks; j++) {
          float latitude1 = piDivStacks * j - PIDiv2;
          float sinLat1 = sin(latitude1);
          float cosLat1 = cos(latitude1);
          for (int i = 0; i <= slices; i++) {
            float longitude = (PI2 / slices) * i;
            float sinLong = sin(longitude);
            float cosLong = cos(longitude);
            vec3 normal = vec3(cosLong * cosLat1, sinLat1, sinLong * cosLat1);
            vec3 position = normal * radius;
            //Vertex v = {position, vec2(j/(float)stacks, i/(float)slices), normal};
            Vertex v = {position, vec3(0.3f, 0.6f, 0.2f), normal};
            vertexData.push_back(v);
          }
        }
        ab = SharedArrayBuffer(new ArrayBuffer());
        ab->defineAttribute("aPosition", GL_FLOAT, 3);
        ab->defineAttribute("aColor", GL_FLOAT, 3);
        ab->defineAttribute("aNormal", GL_FLOAT, 3);
        ab->setDataElements(vertexData.size(), value_ptr(vertexData[0].position), GL_DYNAMIC_DRAW);

        vao = SharedVertexArrayObject(new VertexArrayObject());
        vao->attachAllAttributes(ab);
        vertexDataTest = vertexData;
      };

    ACGL::OpenGL::SharedVertexArrayObject getVAO() { return vao; }

    void render(ACGL::OpenGL::SharedShaderProgram shader, mat4 *viewProjectionMatrix){

      ab->setSubData(0, vertexDataTest.size() * sizeof(Vertex), value_ptr(vertexDataTest[0].position));
      mat4 modelMatrix = translate(getPosition()) * getRotation() *
        scale<float>(vec3(0.4f));
      shader->setUniform("uModelMatrix", modelMatrix);

      mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
      shader->setUniform("uMVP",         mvp);

      // Draw the triangle !
      vao->render(); // Starting from vertex 0; 3 vertices -> 1 triangle
    }
};
