#pragma once

/**
 * Helper function for writing the contents of a Texture object into a file
 * and loading them from a file.
 */

#include <ACGL/ACGL.hh>
#include <ACGL/OpenGL/Data/ColorSpace.hh>
#include <ACGL/OpenGL/Objects/Texture.hh>

#include <string>

namespace ACGL{
namespace OpenGL{

//! loads the texture and creates mip maps
SharedTextureCubeMap loadTextureCubeMap(const std::vector<std::string>& _filename, ColorSpace _colorSpace = ColorSpace::AUTO_DETECT);


}
}
