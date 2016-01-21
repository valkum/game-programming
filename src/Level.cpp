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
    json cameraJson = levelJson["camera"];
    camera = new ACGL::Scene::GenericCamera();
    camera->setPosition(parseVec3(cameraJson["position"]));
    camera->setRotationMatrix(parseMat3(cameraJson["rotation"]));
  } catch (exception e) {
    error() << "parsing" << std::endl;
  }
}

