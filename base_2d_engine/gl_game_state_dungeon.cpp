#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <functional>
#include <utility>
#include <algorithm>
//#define GLM_SWIZZLE_XYZW

#include "glm/glm.hpp"

#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "gl_physics.h"
#include "gl_game_state_dungeon.h"

#include "loader.h"
#include "engine_settings.h"





GlGameStateDungeon::GlGameStateDungeon(std::map<const std::string,GLuint> &shader_map,
                                    std::map<std::string,std::shared_ptr<glRenderTarget>> & render_target_map,
                                    GLResourcesManager &resources_manager,
                                    size_t screen_width,
                                    size_t screen_height):
                                                        IGlGameState(shader_map,resources_manager,screen_width,screen_height)
                                                        ,m_render_target_map(render_target_map)
                                                        ,m_antialiase_enabled(true)
                                                        ,now_frame(91)
                                                        ,key_angle(0.0f)
{
    m_gl_text = std::make_shared<GlText16x16>("font3.png",GetResourceManager()->m_texture_atlas,0.1f,0.1f);
    chip_texture = GetResourceManager()->m_texture_atlas.Assign("chip1.png");
    float a_ratio = screen_width;
    a_ratio /= screen_height;
    m_map_browser = new Gl2D::GlMapBrowser(-1,-1,2,2,a_ratio,GetResourceManager()->m_texture_atlas.Assign("chip1.png"),m_shader_map["sprite2dsimpleBlur"]);
    m_map_browser->SetItemAligment(Gl2D::ItemAligment::Center);
    m_map_browser->SetAspectRatioKeeper(Gl2D::AspectRatioKeeper::Minimal);

    // m_message_processor.Add("teleport",[this](std::stringstream &sstream)
    //                                 {
    //                                     std::string level;
    //                                     std::string start;
    //                                     sstream >> level >> start;
    //                                     LoadMap(level,start);
    //                                 });
    // m_message_processor.Add("spawn",[this](std::stringstream &sstream)
    //                                 {
    //                                     std::string object;
    //                                     std::string name;
    //                                     glm::vec3 position;
    //                                     sstream >> object >> name >> position;
    //                                     AddObjectFromFile(object,name,position);
    //                                 });

    // m_message_processor.Add("rotate",[this](std::stringstream &sstream)
    //                                 {
    //                                     std::string name;
    //                                     sstream >> name;
    //                                     auto object = FindSharedCollectionByName(dungeon_objects.begin(), dungeon_objects.end(),name);
    //                                     if(object != nullptr)
    //                                     {
    //                                         float angle =0.0f;
    //                                         sstream >>angle;
    //                                         object->model_matrix = glm::rotate(object->model_matrix, glm::radians(angle), LoaderUtility::GetFromStream<glm::vec3>(sstream));
    //                                         object->RefreshMatrixes();
    //                                     }
    //                                 });
    
    // m_message_processor.Add("play_ani",[this](std::stringstream &sstream)
    //                             {
    //                                 std::string name;
    //                                 sstream >> name;
    //                                 auto object = FindSharedCollectionByName(dungeon_objects.begin(), dungeon_objects.end(),name);
    //                                 if(object != nullptr)
    //                                 {
    //                                     object->UseSequence(LoaderUtility::GetFromStream<std::string>(sstream));
    //                                 }
    //                             });                         

    // m_message_processor.Add("hero_strike",[this](std::stringstream &sstream)
    // {                                
    //     mob_events.push_back(AddStrike(hero->model_matrix,hero->GetPosition()));
    // });

    // m_message_processor.Add("hero_use",[this](std::stringstream &sstream)
    // {                                
    //     mob_events.push_back(AddUse(hero->model_matrix,hero->GetPosition()));
    // });

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    time = glfwGetTime();
}

GlGameStateDungeon::~GlGameStateDungeon()
{
    delete(m_map_browser);
}




void GlGameStateDungeon::Draw2D(GLuint depth_map)
{
    
    glClear( GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    m_map_browser->Draw();

    const float text_size_y = 0.060f;
    const float text_size_x = m_aspect_ratio * text_size_y;

    m_gl_text->SetTextSize(text_size_x,text_size_y); 
    auto shader = m_shader_map["sprite2dsimple"];
    std::stringstream ss;
    ss<< std::fixed<<std::setprecision(1)<<EngineSettings::GetEngineSettings() ->GetFPS()<<" FPS";
    m_gl_text->DrawString(ss.str(),-1.0f,1.0f - text_size_y*1.2f, shader);
}

void GlGameStateDungeon::Draw()
{
  

    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>(m_render_target_map["base_deffered"].get()));
    glRenderTarget &final_render_target = *(m_render_target_map["final"].get());
    glRenderTarget &postprocess_render_target = *(m_render_target_map["postprocess"].get());

    size_t width = IGlGameState::m_screen_width;
    size_t height = IGlGameState::m_screen_height;

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    GLuint current_shader = m_shader_map["fullscreen"];

    glUseProgram(current_shader);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postprocess_render_target.AlbedoMap);
    renderQuad();/**/

    Draw2D(render_target.depthMap);

    glEnable(GL_DEPTH_TEST);


}



void GlGameStateDungeon::PostMessage(const std::string & event_string)
{
    m_messages.push_back(event_string);
}

void GlGameStateDungeon::ProcessMessages()
{
    while (!m_messages.empty())
    {
        m_message_processor.Process(m_messages.front());
        m_messages.pop_front();
    }
}


IGlGameState *  GlGameStateDungeon::Process(std::map <int, bool> &inputs, float joy_x, float joy_y)
{
    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>(m_render_target_map["base_deffered"].get()));
    


   
    GLuint current_shader;


    double time_now = glfwGetTime();
    //std::cout<<(time_now - time)<<'\n';
    if((time_now - time)>(1.0/30.0))
    {
        processed = true;
        bool moving = inputs[GLFW_KEY_RIGHT]|inputs[GLFW_KEY_DOWN]|inputs[GLFW_KEY_LEFT]|inputs[GLFW_KEY_UP];
        if(moving)
        {
            float x =0;
            float y = 0;

            if(inputs[GLFW_KEY_RIGHT]) x += 0.01f;
            if(inputs[GLFW_KEY_LEFT]) x -= 0.01f;

            if(inputs[GLFW_KEY_DOWN]) y += 0.01f;
            if(inputs[GLFW_KEY_UP]) y -= 0.01f;

            m_map_browser->MoveMap(x,y);      
        }

        if(inputs[GLFW_KEY_LAST+3])
        {
            m_map_browser->SetZoom(2.0f * m_map_browser->GetZoom());
            
            if(m_map_browser->GetZoom()> 16.0f)
                m_map_browser->SetZoom(1.0f);

            inputs[GLFW_KEY_LAST+3] = false;
        }

        ProcessMessages();

   
        // distance = glm::clamp(distance,3.0f,14.0f);

        time = time_now;
        //bool fast_move = inputs[GLFW_KEY_LEFT_SHIFT];

        // m_messages.push_back("hero_use");

        
    }
    return this;
}
