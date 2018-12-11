#include "map_heightmap.h"

namespace GameMap
{

    HeightMap::~HeightMap()
    {
        if(m_heightmap != nullptr)
        {
            DeleteHeightMap(m_heightmap);
        }
    }
    void HeightMap::LoadMap(std::string FileName)
    {
        m_heightmap = LoadHeightMap(GetResourceManager()->m_texture_atlas.GetResourceFolder() + FileName,&m_width, &m_height);
        
        m_heightmap_texture = GetResourceManager()->m_texture_atlas.Assign(FileName);
    }

    void HeightMap::SetParameters(float map_scaler,float height_scaler)
    {
        m_map_scaler = map_scaler;
        m_height_scaler = height_scaler;
    }
    
    float HeightMap::GetHeight(float x,float y)
    {
        if(m_heightmap == nullptr)
            return 0.0f;

         

        int ix = (x * m_map_scaler + 0.5f) * m_width;
        int iy = (y * m_map_scaler + 0.5f) * m_height;

        ix %= m_width;
        iy %= m_height;

        if(ix < 0) ix+= m_width;
        if(iy < 0) iy+= m_height;

        size_t offset = iy; 
        offset = (offset * m_width + ix);
        offset *= 4;
        
        unsigned char height = m_heightmap[offset];
        return 0.00392157f * height * m_height_scaler;
    }

    float HeightMap::GetMapScaler()
    {
        return m_map_scaler;
    }

    float HeightMap::GetHeightScaler()
    {
        return m_height_scaler;
    }
}