#ifndef LEVEL_HH
#define LEVEL_HH

#include "world/Object.hh"
#include "world/Terrain.hh"
#include "world/SkyDome.hh"
#include <ACGL/Scene/GenericCamera.hh>

class Level {
public:
  Level(std::string levelId);
  ~Level();
  void load();
protected:
private:
  std::string levelId;
  std::vector<Object*> levelObjects;
  //Cloth cloth;
  //Clouds clouds;
  Entity* character;
  SkyDome* skyDome;
  ACGL::Scene::GenericCamera* camera;
  Terrain* terrain;
  //Light settings
};


#endif // LEVEL_HH
