#ifndef GL_MAP_HEIGHTMAP_H
#define GL_MAP_HEIGHTMAP_H
#include "gl_resources_manager.h"
#include <memory>

namespace GameMap
{
    class HeightMap
    {

        private:
        std::shared_ptr<IGlTextureStruct> heightmap_texture;
    };
    
}

#endif