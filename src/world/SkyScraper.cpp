#include "world/SkyScraper.hh"

class SkyScraper;

SkyScraper::SkyScraper(vec3 position, vec3 rotation) : Object(new Model("SkyScraper.obj", 0.5f), position, rotation) {

} 