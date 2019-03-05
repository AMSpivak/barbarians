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

    size_t inline MapOffset(size_t ix,size_t iy, size_t line_width, size_t lines_count, size_t pixel_size)
    {
        if(ix < 0) ix+= line_width;
        if(iy < 0) iy+= lines_count;

        ix %= line_width;
        iy %= lines_count;

        size_t offset = iy; 
        offset = (offset * line_width + ix);
        return offset * pixel_size;
    }
    
    float HeightMap::GetHeight(float x,float y)
    {
        if(m_heightmap == nullptr)
            return 0.0f;

        float fx =(x * m_map_scaler + 0.5f) * m_width;
        float fy =(y * m_map_scaler + 0.5f) * m_height;
         

        int ix = fx;
        int iy = fy;

        ix %= m_width;
        iy %= m_height;

        float off_x = fx - ix;
        float off_y = fy - iy;

        size_t  offset = MapOffset(ix,iy,m_width,m_height,4);    
        float h0 = 0.00392157f * m_heightmap[offset] * m_height_scaler;
        offset = MapOffset(ix+1,iy,m_width,m_height,4);    
        float hx = 0.00392157f * m_heightmap[offset] * m_height_scaler;
        offset = MapOffset(ix,iy+1,m_width,m_height,4);    
        float hy = 0.00392157f * m_heightmap[offset] * m_height_scaler;
        offset = MapOffset(ix+1,iy+1,m_width,m_height,4);    
        float hxy = 0.00392157f * m_heightmap[offset] * m_height_scaler;
        float sx1 = h0 + off_x * (hx - h0);
        float sx2 = hy + off_x * (hxy - hy);

        return sx1 + off_y * (sx2 - sx1);
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