#include "Level.hh"
#include <json.hpp>

#include <ACGL/Utils/Log.hh>


using json = nlohmann::json;
using namespace ACGL::Base;
using namespace ACGL::Utils;
class Level;

Level::Level(std::string levelId) : levelId(levelId) {

}
void Level::load(){
  std::string levelDir = Settings::the()->getResourcePath() + "levels/";
  std::fstream levelFile;
  
  try {
    levelFile.open(levelDir + levelId + ".lvl");
    if(levelFile.is_open()) {
      json levelJson;
      try {
        levelFile >> levelJson;

        debug() << levelJson["name"] << std::endl;
      } catch(exception e) {
        debug() << "File is no valid JSON." << std::endl;    
        debug() << levelJson << std::endl;
      }
    }else {
      error() << "File: " << levelDir + levelId + ".lvl could not be opened." << std::endl;
    }
  }catch(exception e) {
    debug() << "File could not be opened." << std::endl;    
  }

  

  
}