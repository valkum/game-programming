#include "TextureLoadStore.h"
#include <ACGL/OpenGL/Data/TextureDataLoadStore.hh>

namespace ACGL {
namespace OpenGL {
SharedTextureCubeMap loadTextureCubeMap(const std::vector<std::string>& _filename, ColorSpace _colorSpace)
{
    SharedTextureCubeMap texture;
    // It is a cube map texture
    texture = SharedTextureCubeMap(new TextureCubeMap(GL_RGB));
    texture->bind();
    SharedTextureData data;
    for(GLenum i = 0; i < 6; ++i) {

        data = loadTextureData(_filename[i], _colorSpace);
        if(data)
        {
            // unsigned int numMipmapLevels = 1000;
        
            GLenum target = GL_INVALID_ENUM;
            switch(i)
            {
                case 0: target = GL_TEXTURE_CUBE_MAP_POSITIVE_X; break;
                case 1: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X; break;
                case 2: target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y; break;
                case 3: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; break;
                case 4: target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z; break;
                case 5: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; break;
            }
            texture->setImageData(data, target, 0);
            // glTexImage2D(target, 0, GL_RGB, data->getWidth(), data->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            //ACGL::Utils::error() << "could not open " << _filename << std::endl;
            std::cout<<"could not open " << _filename[i] << std::endl;
        }
    }
    return texture;
}

}}