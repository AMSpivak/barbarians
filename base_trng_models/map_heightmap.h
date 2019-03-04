#ifndef GL_MAP_HEIGHTMAP_H
#define GL_MAP_HEIGHTMAP_H
#include "gl_resources_manager.h"
#include <memory>

namespace GameMap
{
    class HeightMap
    {
        public:
        HeightMap():m_heightmap(nullptr){}

        ~HeightMap();
        void LoadMap(std::string FileName);
        void SetParameters(float map_scaler,float height_scaler);
        float GetHeight(float x,float y);
        float GetMapScaler();
        float GetHeightScaler();

        std::shared_ptr<IGlTextureStruct> m_heightmap_texture;
        std::shared_ptr<IGlTextureStruct> m_texture;

        private:
        float m_map_scaler;
        float m_height_scaler;        
        int m_height;
        int m_width;

        unsigned char * m_heightmap;
    };
    
}

#endif