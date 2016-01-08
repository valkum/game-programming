#include "world/Terrain.hh"
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

using namespace ACGL::OpenGL;

class Terrain;

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

const int mapSize = 8;

const int height[mapSize][mapSize] ={
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 3, 2, 3, 3, 4, 2, 1 },
  { 1, 2, 1, 2, 2, 2, 2, 1 },
  { 1, 2, 1, 2, 2, 2, 2, 1 },
  { 1, 2, 1, 2, 2, 2, 2, 1 },
  { 1, 2, 2, 3, 3, 3, 3, 2 },
  { 1, 2, 1, 1, 1, 1, 1, 1 } };



void Terrain::init() {
  SharedArrayBuffer ab = SharedArrayBuffer(new ArrayBuffer());
  vao = SharedVertexArrayObject(new VertexArrayObject());
  
  ab->defineAttribute("aPosition", GL_FLOAT, 3);
  ab->defineAttribute("aTexCoord", GL_FLOAT, 2);
  ab->defineAttribute("aNormals", GL_FLOAT, 3);
  
  vao->setMode(GL_TRIANGLE_FAN);
  int floatsPerVertex = 8;
  int numVertices = mapSize*mapSize;
  float* abData = new float[numVertices*floatsPerVertex];
  int dataCount = 0;
  for (int x = 0; x < mapSize; x++) { 
    for (int z = 0; z < mapSize; z++) {
      abData[dataCount++] = (float)x;
      abData[dataCount++] = (float)height[x][z];
      abData[dataCount++] = (float)z;

      abData[dataCount++] = (float)x;
      abData[dataCount++] = (float)z;

      abData[dataCount++] = (float)0.0;
      abData[dataCount++] = (float)1.0;
      abData[dataCount++] = (float)0.0;
    }
  }
  ab->setDataElements(numVertices, abData);
  delete[] abData;

  vao->attachAllAttributes(ab);
  
}

void Terrain::render(ACGL::OpenGL::SharedShaderProgram shader, 
glm::mat4 *viewProjectionMatrix) {
  shader->use();
  vao->render();
}
