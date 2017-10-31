#include "gl_spinner.h"
#include <math.h>  



GlSpinner::GlSpinner(GLTextureAtlas &texture_atlas,size_t sectors)
                    : m_texture_atlas(texture_atlas)
{
    m_sectors = sectors > 6 ? sectors : 6;
    textures.emplace_back(m_texture_atlas.AssignTexture("material/cards/ace_of_spades.png"));
    textures.emplace_back(m_texture_atlas.AssignTexture("material/cards/ace_of_hearts.png"));
}

void GlSpinner::Draw(GLuint spriteShader,float angle,float x_start, float x_end)
{
    angle *= PI/180.0;

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float sector_angle = PI * 2.0f / m_sectors;
    float scaler = 1.0/sin(sector_angle * 1.5f);
    

    for(int sector_index = 0; sector_index < m_sectors; sector_index++)
    {
        float y_top = scaler*sin(angle + sector_angle * sector_index);
        float y_bottom = scaler*sin(angle + sector_angle * (sector_index - 1));

        float x_top = cos(angle + sector_angle * sector_index);
        float x_bottom = cos(angle + sector_angle * (sector_index - 1));

        if((x_top>0)||(x_bottom>0))
        {
          
            renderSprite(spriteShader,x_start,y_bottom,x_end,y_bottom,x_end,y_top,x_start,y_top,
                            glm::vec4(1.0f,1.0f,1.0f,1.0f),
                            textures[(sector_index&0x1)].get());
        }
    }
}