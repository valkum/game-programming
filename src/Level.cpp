#include "Level.hh"

#include <ACGL/Utils/Log.hh>

using namespace ACGL::Base;
using namespace ACGL::Utils;
class Level;

Level::Level(std::string levelId) : levelId(levelId) {

}
void Level::load(){
  std::string levelDir = Settings::the()->getResourcePath() + "levels/";
  std::fstream levelFile;
  json levelJson;

    levelFile.open(levelDir + levelId + ".lvl");
    if(levelFile.is_open()) {
      try {
        levelFile >> levelJson;
      } catch(exception e) {
        error() << "File " << levelDir + levelId + ".lvl could not be parsed." << std::endl;
      }
    }else {
      error() << "File: " << levelDir + levelId + ".lvl could not be opened." << std::endl;
    }

  try {
    camera = new ACGL::Scene::GenericCamera();
    camera->setPosition(parseVec3(levelJson["camera"]["position"]));
    camera->setRotationMatrix(parseMat3(levelJson["camera"]["rotation"]));

    skyDome = new SkyDome(Model("SkyDome.obj", levelJson["skydome"]["scale"]), levelJson["skydome"]["texture"]);
    terrain = new Terrain(levelJson["mapSize"].at(0), levelJson["mapSize"].at(1));

    for (auto object : levelJson["objects"]) {
      levelObjects.push_back(new Object(Model(object["model"], object["scale"]), parseVec3(object["position"]), parseVec3(object["rotation"])));
    }

    clouds = new Clouds(levelJson["clouds"].at(0), levelJson["clouds"].at(1), levelJson["mapSize"].at(0), levelJson["mapSize"].at(1));
    globalWind = parseVec3(levelJson["globalWind"]);

  } catch (exception e) {
    error() << "parsing" << std::endl;
  }
}

