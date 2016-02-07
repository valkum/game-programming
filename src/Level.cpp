#include "Level.hh"

#include <ACGL/Utils/Log.hh>
#include <unordered_map>


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

    std::unordered_map<string, std::pair<ACGL::OpenGL::SharedVertexArrayObject, GLsizei>> geometries;
    for (auto object : levelJson["objects"]) {
      auto search = geometries.find(object["model"]);
      GLsizei primCount = 1;
      if(search != geometries.end()){
        search->second.second++;
        primCount = search->second.second;
        levelObjects.push_back(new Object(Model(search->second.first, object["scale"], primCount), parseVec3(object["position"]), parseVec3(object["rotation"])));
      } else {
        auto vao = ACGL::OpenGL::VertexArrayObjectCreator(object["model"].get<string>()).create();
        geometries.insert(std::pair<string, std::pair<ACGL::OpenGL::SharedVertexArrayObject, GLsizei>>(object["model"].get<string>(), std::pair<ACGL::OpenGL::SharedVertexArrayObject, GLsizei>(vao, 1)));
        levelObjects.push_back(new Object(Model(vao, object["scale"], primCount), parseVec3(object["position"]), parseVec3(object["rotation"])));
      }
      
    }

    clouds = new Clouds(levelJson["clouds"].at(0), levelJson["clouds"].at(1), levelJson["mapSize"].at(0), levelJson["mapSize"].at(1));
    globalWind = parseVec3(levelJson["globalWind"]);

  } catch (exception e) {
    error() << "parsing" << std::endl;
  }
}

bool Level::boundingBoxCollision(boundingBox a, boundingBox b){
  return (a.maxX >= b.minX && a.minX <= b.maxX)
      && (a.maxY >= b.minY && a.minY <= b.maxY)
      && (a.maxZ >= b.minZ && a.minZ <= b.maxZ);
}

std::string Level::boundingBoxToString(boundingBox bBox){
  return "minX: " + std::to_string(bBox.minX) 
     + "; maxX: " + std::to_string(bBox.maxX) 
     + "; minY: " + std::to_string(bBox.minY) 
     + "; maxY: " + std::to_string(bBox.maxY) 
     + "; minZ: " + std::to_string(bBox.minZ) 
     + "; maxZ: " + std::to_string(bBox.maxZ) + ";";
}

bool Level::collisionDetection(vec3 charPos, vec3 charRotation, float charScale){
  boundingBox charBox = {
    charPos[0] - charScale * 1.962f, //minX
    charPos[0] + charScale * 2.031f, //maxX
    charPos[1] - charScale * 4.369f, //minY
    charPos[1] + charScale * 2.453f, //maxY
    charPos[2] - charScale * 6.887f, //minZ
    charPos[2] + charScale * 6.800f  //maxZ
  };

  vec3 buildingPos= vec3(0.0f, 0.0f, 0.0f);
  //vec3 buildingRotation = vec3(0.0f, 0.0f, 0.0f);
  float buildingScale = 0.0f;

  for(auto const& building: levelObjects){
    buildingPos = building->getPosition();
    buildingScale = building->getModel().getScale();


    boundingBox buildingBox = {
      buildingPos[0] - buildingScale * 0.974f, //minX
      buildingPos[0] + buildingScale * 1.025f, //maxX
      buildingPos[1] - buildingScale * 0.000f, //minY
      buildingPos[1] + buildingScale * 6.933f, //maxY
      buildingPos[2] - buildingScale * 0.982f, //minZ
      buildingPos[2] + buildingScale * 1.017f  //maxZ
    };


    if(boundingBoxCollision(charBox, buildingBox)){
      //cout << boundingBoxToString(charBox) << endl;
      //cout << boundingBoxToString(buildingBox) << endl;
      //cout << std::to_string(charBox.minZ - charBox.maxZ) << endl;
      //cout << "===============" << endl;
      return true;
    }
  }
  return false;
}
