#include "world/Terrain.hh"

#include <ACGL/Utils/Log.hh>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

using namespace glm;
using namespace ACGL::OpenGL;
using namespace ACGL::Utils;

class Terrain;

struct Vertex {
            glm::tvec3<float> position;
            glm::tvec3<float> uv;
            glm::tvec3<float> normal;
        };

/* calc height from greyscale bmp pixel per vertex
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
 
    switch(bpp) {
    case 1:
        return *p;
 
    case 2:
        return *(Uint16 *)p;
 
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
 
    case 4:
        return *(Uint32 *)p;
 
    default:
        return 0;     
    }
}
*/

//create heightmap
//todo: replace with greyscale bmp?
/* example plagiat
if ((TextureImage[0] = SDL_LoadBMP("terrain.bmp"))) {
 for (x = 1; x < MAP_W; x++) { 
  for (y = 1; y < MAP_H; y++) {
   height[x][y] = variation * ( get_pixel_normalized(TextureImage[0],x,y) ) ;  }
 }
}
*/


const int height[8][8] ={
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 3, 2, 3, 3, 4, 2, 1 },
  { 1, 2, 1, 2, 2, 2, 2, 1 },
  { 1, 2, 1, 2, 2, 2, 2, 1 },
  { 1, 2, 1, 2, 2, 2, 2, 1 },
  { 1, 2, 2, 3, 3, 3, 3, 2 },
  { 1, 2, 1, 1, 1, 1, 1, 1 } };


/**
 * Use random method for heightfield.
 */
Terrain::Terrain() {
  
  //vao->setMode(GL_TRIANGLE_STRIP);
  // int floatsPerVertex = 8;
  std::vector<Vertex> vertexData;
  for (int x = 0; x < 7; x++) { 
    for (int z = 0; z < 7; z++) {
      //first triangle of quad
      Vertex v = {glm::vec3(x, height[x][z], z), glm::vec3(x/8, z/8, 0), glm::vec3(0.0f, 1.0f, 0.0f)};
      vertexData.push_back(v);
      v = {glm::vec3(x+1, height[x+1][z], z), glm::vec3(x+1/8, z/8, 0), glm::vec3(0.0f, 1.0f, 0.0f)};
      vertexData.push_back(v);
      v = {glm::vec3(x, height[x][z+1], z+1), glm::vec3(x/8, z+1/8, 0), glm::vec3(0.0f, 1.0f, 0.0f)};
      vertexData.push_back(v);
      //second triangle of quad
      v = {glm::vec3(x+1, height[x+1][z], z), glm::vec3(x+1/8, z/8, 0), glm::vec3(0.0f, 1.0f, 0.0f)};
      vertexData.push_back(v);
      v = {glm::vec3(x, height[x][z+1], z+1), glm::vec3(x/8, z+1/8, 0), glm::vec3(0.0f, 1.0f, 0.0f)};
      vertexData.push_back(v);
      v = {glm::vec3(x+1, height[x+1][z+1], z+1), glm::vec3(x+1/8, z+1/8, 0), glm::vec3(0.0f, 1.0f, 0.0f)};
      vertexData.push_back(v);
    }
  }
  debug() << vertexData.size() << std::endl;
  SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aTexCoord", GL_FLOAT, 3);
  ab->defineAttribute("aNormals", GL_FLOAT, 3);
  ab->setDataElements(vertexData.size(), glm::value_ptr(vertexData[0].position), GL_DYNAMIC_DRAW);

  vao = SharedVertexArrayObject(new VertexArrayObject());
  vao->attachAllAttributes(ab);

}

/**
 * Update ArrayBuffer. This way we only render terrain in a defined radius.
 *
 */
void Terrain::render(ACGL::OpenGL::SharedShaderProgram shader, glm::mat4 *viewProjectionMatrix) {
  glm::mat4 modelMatrix = glm::translate(glm::vec3(0.0f,0.0f,0.0f)) * glm::scale<float>(glm::vec3(10.0f));
  glm::mat4 mvp = (*viewProjectionMatrix) * modelMatrix;
  shader->setUniform("uMVP",         mvp);

  vao->render();
}
