#include "gl_map_browser.h"
#include <algorithm>
#include "glresourses.h"
#include "glm/glm.hpp"


namespace Gl2D
{
    void GlMapBrowser::Draw()
    {
        RecalculateGeometry();
        float tile_size = 0.5f * m_zoom;

        float x_off = 0.5f - m_map_x - tile_size;
        float y_off = 0.5f - m_map_y - tile_size;
        tile_size = m_zoom;

        renderSprite(m_shader,
			real_x, real_y,
			real_x + real_width, real_y, 
			real_x + real_width, real_y + real_height,
			real_x, real_y + real_height,
			glm::vec4(1.0f,1.0f,1.0f,1.0f),&(m_map_texture->m_texture),
			tile_size, tile_size,
			x_off,y_off
			);
    }
    
    void GlMapBrowser::SetZoom(float value)
    {
        if(value > 0.001f && value < 1000.f)
        {
            m_zoom = 1.0/value;
        }
    }

    float GlMapBrowser::GetZoom()
    {
        return 1.0/m_zoom;
    }

    void GlMapBrowser::MoveMap(float x, float y)
    {
        m_map_x -= m_zoom * x;
        m_map_x = glm::clamp(m_map_x,-0.5f,0.5f);
        m_map_y -= m_zoom * y;
        m_map_y = glm::clamp(m_map_y,-0.5f,0.5f);
    }

}