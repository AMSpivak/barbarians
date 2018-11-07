#include "gl2d_progressbar.h"
#include <algorithm>
#include "glresourses.h"
#include "glm/glm.hpp"


namespace Gl2D
{
    void GlProgressbar::Draw()
    {
        RecalculateGeometry();


        renderSprite(m_shader,
			real_x, real_y,
			real_x + real_width, real_y, 
			real_x + real_width, real_y + real_height,
			real_x, real_y + real_height,
			glm::vec4(1.0f,1.0f,1.0f,1.0f),&(m_fill_texture->m_texture),
			m_progress, 1.0f,
			0.0f,0.0f
			);
        renderSprite(m_shader,
			real_x, real_y,
			real_x + real_width, real_y, 
			real_x + real_width, real_y + real_height,
			real_x, real_y + real_height,
			glm::vec4(1.0f,1.0f,1.0f,1.0f),&(m_bar_texture->m_texture),
			1.0f, 1.0f,
			0.0f,0.0f
			);
    }
    
    void GlProgressbar::SetProgress(float value)
    {
        if(value >= 0.0f && value <= 1.f)
        {
            m_progress = value;
        }
    }


}