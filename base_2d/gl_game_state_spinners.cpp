#include "gl_game_state_spinners.h"
#include <GLFW/glfw3.h>
#include <math.h>

const size_t sectors_count = 10;

GlGameStateSpinner::GlGameStateSpinner(std::map<std::string,GLuint> &shader_map,
    GLTextureAtlas &texture_atlas,
    size_t screen_width,
    size_t screen_height):
    IGlGameState(shader_map,texture_atlas,screen_width,screen_height)
    ,spinner(texture_atlas,sectors_count)
    ,time(0.0)
    ,spinning_count(0)
{
    
}


void GlGameStateSpinner::Draw()
{
    //std::shared_ptr<GLuint> cursor = m_texture_atlas.AssignTexture("material/cursor.png");
    //std::shared_ptr<GLuint> card_ace = m_texture_atlas.AssignTexture("material/cards/ace_of_spades.png");
    static GLuint shader = 0;
    if(shader == 0) shader = m_shader_map["sprite"];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i =0;i<5;i++)
    {
        spinner.Draw(shader,angles[i],-1.0f + i * 0.4f,-0.6f+ i * 0.4f);
    }
    
}

IGlGameState * GlGameStateSpinner::Process(std::map <int, bool> &inputs)
{
    float sector_angle = (360.f/sectors_count);

    double time_now = glfwGetTime();
    if(!spinning_count)
    {
        time = time_now;
        if(inputs[GLFW_KEY_SPACE])
        {
            spinning_count = 5;
            for(int i =0;i<spinning_count;i++)
            {
                angles[i] = 0.0f;
                speeds[i] = 200.0f - i * 30.0f;
            }
        }
    }

    if(spinning_count)
    {
        for(int i =0;i<spinning_count;i++)
        {
            angles[i]+= (time_now - time) * speeds[i];

            if(angles[i]>360.0) angles[i] -=360.0f;
            speeds[i] -=(time_now - time) * 10.1f;

            if(speeds[i]<0.0f) 
            {
                speeds[i] = 0;
                float pos = angles[i] /sector_angle - 0.5f;
                float r_pos = round(pos);

                //std::cout<<"\n"<<pos<<" "<<r_pos<<"\n";

                if(std::abs(pos - r_pos) < 0.001f)
                {
                    //std::cout<<"\n stop \n";
                    spinning_count--;
                    angles[i] = sector_angle * 0.5 + sector_angle* r_pos;
                }
                else
                {
                    //std::cout<<"\n spin \n";
                    
                    angles[i] = (0.9f*angles[i] + sector_angle * 0.05f + 0.1f*sector_angle* r_pos);
                }
            }
        }

        time = time_now;
    }
    
}