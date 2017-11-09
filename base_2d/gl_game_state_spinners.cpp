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
    textures.emplace_back(m_texture_atlas.AssignTexture("material/explosion.png"));
    textures.emplace_back(m_texture_atlas.AssignTexture("material/playnow.png"));
    fx_radius = 0.7f;    
    
}


void GlGameStateSpinner::Draw()
{
    float scale = (float)m_screen_width/m_screen_height;
    /*std::shared_ptr<GLuint> button_texture = m_texture_atlas.AssignTexture("material/cursor.png");
    std::shared_ptr<GLuint> fx_texture = m_texture_atlas.AssignTexture("material/cards/ace_of_spades.png");*/
    static GLuint shader = 0;
    if(shader == 0) shader = m_shader_map["sprite"];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i =0;i<5;i++)
    {
        spinner.Draw(shader,angles[i],-1.0f + i * 0.4f,-0.6f+ i * 0.4f);
    }
    if(!spinning_count)
    {

        float fx_x = fx_radius* cos(fx_angle);
        float fx_y = fx_radius* sin(fx_angle);

        renderSprite(shader,-fx_x,-fx_y*scale,-fx_y,fx_x*scale,fx_x,fx_y*scale,fx_y,-fx_x*scale,
                glm::vec4(1.0f,1.0f,1.0f,1.0f),
                textures[0].get());

        renderSprite(shader,-0.35f,-0.1f*scale,0.35f,-0.1f*scale,0.35f,0.1f*scale,-0.35f,0.1f*scale,
            glm::vec4(1.0f,1.0f,1.0f,1.0f),
            textures[1].get());
                
    }
    
}

IGlGameState * GlGameStateSpinner::Process(std::map <int, bool> &inputs)
{
    float sector_angle = (360.f/sectors_count);
    double time_now = glfwGetTime();

    fx_angle += (time_now - time) * 0.1;
    if(fx_angle >PI *2.0f)
    {
        fx_angle -= PI *2.0f;
    }



    if(!spinning_count)
    {
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
<<<<<<< HEAD
                    
                    angles[i] = (0.9f*angles[i] + sector_angle * 0.05f + 0.1f*sector_angle* r_pos);
=======
                    float s = 0.77f * (time_now - time);
                    angles[i] = (1.0f -s)*angles[i] + s*(sector_angle * 0.5f + sector_angle* r_pos);
>>>>>>> c33a268e8a46de9cc526ec6cad76ca118ec5513f
                }
            }
        }
    }
    time = time_now;
}