#include "world/Terrain.hh"

#include <ACGL/Utils/Log.hh>
#include <glm/gtc/noise.hpp>

using namespace glm;
using namespace ACGL::OpenGL;
using namespace ACGL::Utils;

class Terrain;


/**
 * Use random method for heightfield.
 */
Terrain::Terrain(int x, int y, float a, float b) {
  width = x;
  length = y;
  height = generate(a, b);
  //vao->setMode(GL_TRIANGLE_STRIP);
  // int floatsPerVertex = 8;
  std::vector<Vertex> vertexData;
  for (int x = 0; x < length-1; x++) { 
    for (int z = 0; z < width-1; z++) {
      insertQuad(vertexData, height, x, z);
    }
  }
  debug() << vertexData.size() << std::endl;
  SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aNormal", GL_FLOAT, 3);
  ab->setDataElements(vertexData.size(), glm::value_ptr(vertexData[0].position), GL_DYNAMIC_DRAW);

  vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);
  delete [] height;
}

void Terrain::insertQuad(std::vector<Vertex> &vertexData, float* height, int x, int z) {
  Vertex v;
  vec3 normal;
  vec3 p1, p2, p3, p4;
  p1 = vec3(x, height[index(x,z)], z);
  p2 = vec3(x, height[index(x,z+1)], z+1);
  p3 = vec3(x+1, height[index(x+1, z+1)], z+1);
  p4 = vec3(x+1, height[index(x+1, z)], z);
  //debug()<<height[index(x,z)] << std::endl;
  if(x % 2 == 0) { // Determine in which direction the quad is split ckw or cckw.
    //Calc normals
    normal = calcTriangleNormal(p1, p2, p3);
    v = {p1, normal};
    vertexData.push_back(v);
    v = {p2, normal};
    vertexData.push_back(v);
    v = {p3, normal};
    vertexData.push_back(v);

    normal = calcTriangleNormal(p1,p3,p4);
    v = {p1, normal};
    vertexData.push_back(v);
    v = {p3, normal};
    vertexData.push_back(v);
    v = {p4, normal};
    vertexData.push_back(v);
  } else {
    //calc normals
    normal = calcTriangleNormal(p3, p4, p1);
    v = {p1, normal};
    vertexData.push_back(v);
    v = {p4, normal};
    vertexData.push_back(v);
    v = {p3, normal};
    vertexData.push_back(v);

    normal = calcTriangleNormal(p2, p3, p1);
    v = {p1, normal};
    vertexData.push_back(v);
    v = {p3, normal};
    vertexData.push_back(v);
    v = {p2, normal};
    vertexData.push_back(v);
  }
}

vec3 Terrain::calcTriangleNormal(vec3 pos1, vec3 pos2, vec3 pos3)
    {
        vec3 v1 = pos2-pos1;
        vec3 v2 = pos3-pos1;
        return normalize(cross(v1,v2));
    }


float* Terrain::generate(float a, float b) {
  float* height = new float[width*length];

  float xFactor = 1.0f / (width - 1);
  float yFactor = 1.0f / (length - 1);
  for( int row = 0; row < width; row++ ) {
    for( int col = 0 ; col < length; col++ ) {
      float x = xFactor * row;
      float y = yFactor * col;
      float sum = 0.0f;
      float freq = a;
      float scale = b;

      // Compute the sum for each octave
      for( int oct = 0; oct < 4; oct++ ) {
        glm::vec2 p(x * freq, y * freq);
        float val = glm::perlin(p, glm::vec2(freq)) / scale;
        sum += val;
        // float result = (sum + 1.0f)/ 2.0f;
        // Store in texture buffer
        height[index(row, col)] = sum;
        freq *= 2.0f; // Double the frequency
        scale *= b; // Next power of b
      }
    }
  }
  return height;
}
void Terrain::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix) {
  glm::mat4 modelMatrix = glm::translate(glm::vec3(-width/2,.0f,-2.0f)) * glm::scale<float>(glm::vec3(1.0f));
  glm::mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
  shader->setUniform("uModel", modelMatrix);
  shader->setUniform("uMVP",         mvp);

  vao->render();
}
