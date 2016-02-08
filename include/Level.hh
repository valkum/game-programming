#ifndef LEVEL_HH
#define LEVEL_HH

#include "world/Object.hh"
#include "world/Terrain.hh"
#include "world/Clouds.hh"
#include "world/SkyDome.hh"
#include <ACGL/Scene/GenericCamera.hh>
#include <memory>
#include <json.hpp>
#include <glm/glm.hpp>

using json = nlohmann::json;

struct boundingBox{
  float minX;
  float maxX;
  float minY;
  float maxY;
  float minZ;
  float maxZ;
};
struct Light {
  vec3 direction;
  float ambient;
  vec3 color;
  float diffuse;
  float specular;
};

class Level {
public:
  Level(std::string levelId);
  ~Level();
  void load();
  vec3 parseVec3(json a) { return vec3(a.at(0), a.at(1), a.at(2)); };
  mat3 parseMat3(json a) {
    return mat3(
      vec3(a.at(0).at(0), a.at(0).at(1), a.at(0).at(2)),
      vec3(a.at(1).at(0), a.at(1).at(1), a.at(1).at(2)),
      vec3(a.at(2).at(0), a.at(2).at(1), a.at(2).at(2)));
  };
  ACGL::Scene::GenericCamera* getCamera() { return camera; };
  std::vector<Object *> getObjects() { return levelObjects; };
  SkyDome* getSkydome() { return skyDome; };
  Terrain* getTerrain() { return terrain; };
  Clouds* getClouds() { return clouds; };
  vec3 getWind() { return globalWind; };
  Light getLight() {return light;};
  bool collisionDetection(vec3 charPos, vec3 charRotation, float charScale);
protected:
private:
  std::string levelId;
  std::vector<Object*> levelObjects;
  //Cloth cloth;
  Clouds* clouds;
  vec3 globalWind;
  Entity* character;
  SkyDome* skyDome;
  ACGL::Scene::GenericCamera* camera;
  Terrain* terrain;
  Light light;
  //Light settings
  bool boundingBoxCollision(boundingBox a, boundingBox b);
  std::string boundingBoxToString(boundingBox bBox);
};


#endif // LEVEL_HH
