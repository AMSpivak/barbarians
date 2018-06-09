#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <utility>
//#define GLM_SWIZZLE_XYZW

#include "glm/glm.hpp"

#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glscene.h"
#include "gl_physics.h"
#include "gl_game_state_dungeon.h"
#include "map_event_hero_strikes.h"
#include "map_event_valhalla.h"
#include "map_event_general.h"
#include "collision.h"
#include "loader.h"




void ResetModels(std::vector <std::shared_ptr<glModel> > &Models)
{
    for(auto tmpModel : Models)
    {
        tmpModel->model = glm::rotate(tmpModel->model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        tmpModel->model = glm::rotate(tmpModel->model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
}

GlGameStateDungeon::GlGameStateDungeon(std::map<std::string,GLuint> &shader_map,
                                    std::map<std::string,std::shared_ptr<glRenderTarget>> & render_target_map,
                                    std::map<std::string,std::shared_ptr<IGlModel>> & models_map,
                                    GLResourcesManager &resources_manager,
                                    size_t screen_width,
                                    size_t screen_height):
                                                        IGlGameState(shader_map,resources_manager,screen_width,screen_height)
                                                        ,m_render_target_map(render_target_map)
                                                        ,m_models_map(models_map)
                                                        ,m_antialiase_enabled(true)
                                                        ,m_start_place("")
                                                        ,light_angle (90.0f)
                                                        ,light_radius (20.0f)
                                                        ,now_frame(91)
                                                        ,key_angle(0.0f)
                                                        ,m_dungeon(10,10,1)
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    Camera.SetCameraLocation(glm::vec3(12.0f, 8.485f, -12.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Camera.SetCameraLens(45,(float)screen_width / (float)screen_height,0.1f, 100.0f);
    //debug_texture = resources_manager.m_texture_atlas.Assign("fireball.png");
    time = glfwGetTime();
    LoadMap("levels/test.lvl","base");

}

std::shared_ptr<IMapEvent> GlGameStateDungeon::AddStrike(IGlModel &model,glRenderTargetDeffered &render_target)
{

    std::shared_ptr<IMapEventHeroStrike>e_ptr(new IMapEventHeroStrike(m_shader_map["sprite2d"],render_target.depthMap,&(fx_texture.get()->m_texture),1.0f,1.4f));
    IMapEventHeroStrike & event = *(e_ptr.get());
    event.model_matrix = model.model_matrix;
    event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(0.3f,0.5f,0.0f),glm::vec3(0.5f,2.5f,0.0f)));
    event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(0.5f,2.5f,0.0f),glm::vec3(-0.5f,2.5f,0.0f)));
    event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(-0.5f,2.5f,0.0f),glm::vec3(-0.3f,0.5f,0.0f)));
    event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(-0.3f,0.5f,0.0f),glm::vec3(0.3f,0.5f,0.0f)));
    event.position = model.position;
    return e_ptr;
}

void GlGameStateDungeon::SelectStart(std::vector<std::string> &lines)
{
    size_t y = 0;
    std::string start_name;
    for(auto line : lines)
    {
        size_t x = 0;
        size_t tile = 0;
        std::stringstream ss(line);
        ss >> start_name >> hero_position;
        if(!start_name.compare(m_start_place)) 
            return;
    }
}


void GlGameStateDungeon::LoadTiles(std::vector<std::string> &lines)
{
    size_t y = 0;
    for(auto line : lines)
    {
        size_t x = 0;
        size_t tile = 0;
        std::stringstream ss(line);
        while((x< m_dungeon.Width())&&(!ss.eof()))
        {
            ss >> tile;
            m_dungeon.SetTile(x,y,0,tile);
            ++x;            
        }
        ++y;
        if(y>=m_dungeon.Height())
            return;
    }
}

void GlGameStateDungeon::LoadDungeonObjects(std::vector<std::string> &lines)
{
    size_t y = 0;
    for(auto line : lines)
    {
        size_t x = 0;
        size_t object = 0;
        std::stringstream ss(line);
        while((x<m_dungeon.Width())&&(!ss.eof()))
        {
            ss >> object;
            m_dungeon.SetObject(x,y,0,object);
            ++x;
        }
        ++y;
        if(y>=m_dungeon.Height())
            return;
    }
}

void GlGameStateDungeon::LoadObject(std::vector<std::string> &lines)
{
        std::shared_ptr<IGlModel> object_ptr(new GlCharacter());
        dungeon_objects.push_back(object_ptr);
        GlCharacter & barrel_model =  *(dynamic_cast<GlCharacter*>(object_ptr.get()));
        barrel_model.UpdateFromLines(lines);
 
}

void GlGameStateDungeon::SetDungeonSize(std::vector<std::string> &lines)
{
    std::stringstream ss(lines[0]);
    size_t width = 3;
    size_t height = 3;
    size_t floors = 1;

    ss >> width>> height>>floors;
    m_dungeon = GlDungeon(width,height,floors);

}

void GlGameStateDungeon::SetMapLight(std::vector<std::string> &lines)
{
    if(lines.size()<=1) 
        return;
 

    std::map<std::string,const std::function<void(std::stringstream&)>> execute_funcs;
    execute_funcs.insert(std::make_pair("light_pos",[this](std::stringstream &sstream)
                                        {sstream >> light_position;}));
    execute_funcs.insert(std::make_pair("skybox",[this](std::stringstream &sstream)
                                    {
                                        std::string sky;
                                        sstream >> sky;
                                        GLResourcesManager * resources_manager = GetResourceManager();
                                        skybox = resources_manager->m_texture_atlas.Assign(sky);
                                    }));
    execute_funcs.insert(std::make_pair("camera_lens",[this](std::stringstream &sstream)
                                    {
                                        float f_near = 1.f;
                                        float f_far = 35.0f;
                                        float size = 20.0f;
                                        sstream >> size >> f_near >> f_far; 
                                        Light.SetCameraLens_Orto(-size, size,-size, size,f_near,f_far);                                         
                                        Light2.SetCameraLens_Orto(-size*2, size*2,-size*2, size*2,f_near,f_far);                                         
                                    
                                    }));

    execute_funcs.insert(std::make_pair("light_color",[this](std::stringstream &sstream)
                                    { sstream >> light_color_vector;}));

    std::string info = "";
    for(auto line:lines)
    {
        std::stringstream ss(line);
        ss >> info;
        (execute_funcs[info])(ss);
    }

    light_dir_vector = glm::normalize(light_position);
    Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    Light2.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

}


void GlGameStateDungeon::LoadMapEvent(std::vector<std::string> &lines)
{
    if(lines.size()<=1) 
        return;

    
    std::shared_ptr<MapEventGeneral>e_ptr(new MapEventGeneral(m_shader_map["sprite2d"],0,nullptr,1.0f,1.4f));

    float radius = 0;
    
    std::map<std::string,const std::function<void(std::stringstream&)>> execute_funcs;
    execute_funcs.insert(std::make_pair("position",[e_ptr](std::stringstream &sstream)
                                        {sstream >> e_ptr->position;}));
    execute_funcs.insert(std::make_pair("message",[e_ptr](std::stringstream &sstream)
                                    { 
                                        std::string tmp;
                                        //sstream >> tmp;
                                        std::getline(sstream,tmp);
                                        e_ptr->SetMessage(tmp);
                                        
                                    }));
    execute_funcs.insert(std::make_pair("radius",[e_ptr](std::stringstream &sstream)
                                    { sstream >> e_ptr->radius;}));
    std::string info = "";
    for(auto line:lines)
    {
        std::stringstream ss(line);
        ss >> info;
        (execute_funcs[info])(ss);
    }
    hero_events.push_back(e_ptr);

}


void GlGameStateDungeon::LoadMap(const std::string &filename,const std::string &start_place)
{  
    std::ifstream level_file;
	level_file.open(filename); 
    std::cout<<"Level:"<<filename<<" "<<(level_file.is_open()?"-opened":"-failed")<<"\n";  
    if(!level_file.is_open()) return;
    hero_position = glm::vec3(10.0f,0.0f,10.0f);  
    m_start_place = start_place;
    GLResourcesManager * resources_manager = GetResourceManager();
    hero_events.clear();
    mob_events.clear();
    map_events.clear();
    dungeon_objects.clear();

    
    std::map<std::string,const std::function<void(std::vector<std::string> &lines)>> execute_funcs;
    execute_funcs.insert(std::make_pair("sky",[this](std::vector<std::string> &lines){SetMapLight(lines);}));
    execute_funcs.insert(std::make_pair("models",[this](std::vector<std::string> &lines)
                                        {
                                            for(auto line : lines)
                                            {
                                                Models.emplace_back(std::make_shared<glModel>(line));
                                            }
                                            ResetModels(Models);
                                        }));
    execute_funcs.insert(std::make_pair("starts",[this](std::vector<std::string> &lines){SelectStart(lines);}));
    execute_funcs.insert(std::make_pair("dungeon_params",[this](std::vector<std::string> &lines){SetDungeonSize(lines);}));
    execute_funcs.insert(std::make_pair("dungeon_tiles",[this](std::vector<std::string> &lines){LoadTiles(lines);}));
    execute_funcs.insert(std::make_pair("dungeon_objects",[this](std::vector<std::string> &lines){LoadDungeonObjects(lines);}));
    execute_funcs.insert(std::make_pair("object",[this](std::vector<std::string> &lines){LoadObject(lines);}));
    execute_funcs.insert(std::make_pair("hero_event",[this](std::vector<std::string> &lines){LoadMapEvent(lines);}));

    Models.clear();
    
    
    std::vector<std::string> lines;
    std::string sufix ="";

    while(!level_file.eof())
    {
        sufix = LoaderUtility::FindPrefix(level_file);
        //std::cout<<sufix<<"\n";
        LoaderUtility::LoadLineBlock(level_file,sufix,lines);
        try
        {
            execute_funcs.at(sufix)(lines);
        }
        catch(const std::out_of_range& exp)
        {
            std::cout<<"Unknown prefix: "<<sufix<<"\n";
        }
    }


    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));
    hero.UseSequence("stance");

    



    level_file.close(); 

    fx_texture = resources_manager->m_texture_atlas.Assign("valh.png");  
    GetResourceManager()->Clean();  

}


void GlGameStateDungeon::DrawDungeon(GLuint current_shader,const GlCharacter &hero)
{
    glm::mat4 model_matrix = Models[0]->model;
    glm::mat4 pos_matrix;
    size_t iz = 0;
    for(int iy = 0; iy < m_dungeon.Height(); iy++)
    {
        pos_matrix = glm::mat4();
        pos_matrix = glm::translate(pos_matrix, glm::vec3(0.0f, 0.0f, 2.0f*iy) - hero_position);

        for(int ix = 0; ix < m_dungeon.Width(); ix++)
        {
            int index = m_dungeon.GetMapTilesIndex(ix,iy,iz);
            //m_dungeon_map_tiles[iz*m_dungeon_width*m_dungeon_height + m_dungeon_width*iy +ix];
           
            if(index>=0)
            {

                model_matrix = Models[index]->model;
                Models[index]->model = pos_matrix * model_matrix;
                Models[index]->Draw(current_shader,now_frame);
                Models[index]->model = model_matrix;
            }
            
            index = m_dungeon.GetMapObjectIndex(ix,iy,iz);//m_dungeon_map_objects[iz*m_dungeon_width*m_dungeon_height + m_dungeon_width*iy +ix];
            if(index>0)
            {
                int mod_index = index>>2;
                int rot = index - (mod_index<<2);
                model_matrix = Models[mod_index]->model;
                //model_matrix =  * model_matrix;
                glm::rotate(model_matrix, glm::radians(-90.0f * rot), glm::vec3(0.0f, 1.0f, 0.0f));
                Models[mod_index]->model = pos_matrix * glm::rotate(model_matrix, glm::radians(-90.0f * rot), glm::vec3(0.0f, 0.0f, 1.0f));//model_matrix;
                Models[mod_index]->Draw(current_shader,now_frame);
                Models[mod_index]->model = model_matrix;
            }
            pos_matrix = glm::translate(pos_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
        }
    }
                
    for(auto object : dungeon_objects)
    {  
        auto ptr = object.get();

        pos_matrix = glm::mat4();
        pos_matrix = glm::translate(pos_matrix, ptr->position - hero_position);
        model_matrix = ptr->model_matrix;
        ptr->model_matrix = pos_matrix * model_matrix;
        ptr->RefreshMatrixes();
        ptr->Draw(current_shader);
        ptr->model_matrix = model_matrix;
    }
               
    hero.Draw(current_shader);
}

void DrawSimpleLight(const glm::vec4 &light_pos_vector,const glm::vec3 &light_color_vector,const glm::vec3 &camera_position,GLuint current_shader,glRenderTargetDeffered &render_target)
{
    glClear(GL_DEPTH_BUFFER_BIT);
        //current_shader = m_shader_map["deffered_simple"];
        glUseProgram(current_shader);

		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, render_target.AlbedoMap);

		glUniform1i(glGetUniformLocation(current_shader, "NormalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, render_target.NormalMap);

		glUniform1i(glGetUniformLocation(current_shader, "PositionMap"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, render_target.PositionMap);

        GLuint view_pos  = glGetUniformLocation(current_shader, "viewPos");
		glUniform3fv(view_pos, 1, glm::value_ptr(camera_position));


		GLuint light_pos  = glGetUniformLocation(current_shader, "LightLocation");
		glUniform4fv(light_pos, 1, glm::value_ptr(light_pos_vector));

        
        GLuint light_color  = glGetUniformLocation(current_shader, "LightColor");
        glUniform3fv(light_color, 1, glm::value_ptr(light_color_vector));

        renderQuad();
}

void GlGameStateDungeon::DrawLight(const glm::vec4 &light_pos_vector,const glm::vec3 &light_color_vector,glRenderTargetDeffered &render_target )
{
        //DrawSimpleLight(light_pos_vector,light_color_vector,current_shader,render_target );

    GLuint current_shader = m_shader_map["deffered_simple"];

    glm::vec4 light_position;
    glm::vec3 light_color;
    
    for(std::shared_ptr<IMapEvent> event :map_events) 
    {
        if(event.get()->IsLight(light_position,light_color))
        {
            DrawSimpleLight(light_position - light_pos_vector,light_color,Camera.m_position,current_shader,render_target );
        }
    }

    for(auto object :dungeon_objects) 
    {
        if(object.get()->IsLight(light_position,light_color))
        {
            DrawSimpleLight(light_position - light_pos_vector,light_color,Camera.m_position,current_shader,render_target );
        }
    }
}

void GlGameStateDungeon::DrawFxSprite(GLuint &current_shader, GLuint texture)
{
    glDisable(GL_CULL_FACE);
    if(current_shader == 0)
    {
        current_shader = m_shader_map["sprite"];
    }
    glUseProgram(current_shader);

    glm::mat4 model_m = glm::mat4(1.0f);
    model_m = glm::translate(model_m,glm::vec3(0.5f,0.5f,0.0f));
    glm::mat4 camera_m = glm::mat4(1.0f);
    GLuint cameraLoc  = glGetUniformLocation(current_shader, "camera");
   //glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Camera.CameraProjectionMatrix()));
    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera_m));

    GLuint model_matrix  = glGetUniformLocation(current_shader, "model");
    //glUniformMatrix4fv(model_matrix, 1, GL_FALSE, glm::value_ptr(model_m));
    glUniformMatrix4fv(model_matrix, 1, GL_FALSE, glm::value_ptr(Camera.CameraProjectionMatrix()));
  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    renderQuad();
    glEnable(GL_CULL_FACE);
}


void GlGameStateDungeon::Draw2D(GLuint depth_map)
{
      /*  renderBillBoardDepth(m_shader_map["sprite2d"],depth_map,fx_texture.get(),
        1.0f,1.45f,glm::vec3(2.0f,2.0f,3.0f),hero_position,Camera);
    */

      for(std::shared_ptr<IMapEvent> event :map_events) 
      {
          event.get()->Show(hero_position,Camera);
      }
}
void GlGameStateDungeon::PrerenderLight(glLight &Light,GlCharacter &hero)
{
    Light.SetLigtRender();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.1,4000.0);

    glClear( GL_DEPTH_BUFFER_BIT);
    GLuint current_shader = m_shader_map["shadowmap"];
    glUseProgram(current_shader);
    unsigned int cameraLoc  = glGetUniformLocation(current_shader, "camera");
    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));

    DrawDungeon(current_shader,hero);
    //hero.Draw(current_shader);

    glDisable(GL_POLYGON_OFFSET_FILL);
}

void GlGameStateDungeon::DrawGlobalLight(const GLuint light_loc, const glLight &Light)
{
        //glUniform1i(glGetUniformLocation(current_shader, "shadowMap"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Light.depthMap);

		//GLuint LightLoc  = glGetUniformLocation(current_shader, "lightSpaceMatrix");
		glUniformMatrix4fv(light_loc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));

		renderQuad();
}

void GlGameStateDungeon::Draw()
{

    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>(m_render_target_map["base_deffered"].get()));
    glRenderTarget &final_render_target = *(m_render_target_map["final"].get());
    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));

    size_t width = IGlGameState::m_screen_width;
    size_t height = IGlGameState::m_screen_height;
    int models_count = Models.size();

		glDisable(GL_MULTISAMPLE);
		glDisable(GL_CULL_FACE);

		unsigned int cameraLoc;

		PrerenderLight(Light,hero);
        PrerenderLight(Light2,hero);

		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);


		//*------------------------------
		render_target.set();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);
        glDepthFunc(GL_LEQUAL);


  

		GLuint current_shader = m_shader_map["deff_1st_pass"];
		glUseProgram(current_shader);
		cameraLoc  = glGetUniformLocation(current_shader, "camera");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Camera.CameraMatrix()));

        DrawDungeon(current_shader,hero);
     
		final_render_target.set();
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);


		glViewport(0, 0, width, height);



		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		current_shader = m_shader_map["luminocity"];

        glEnable(GL_STENCIL_TEST);
        glClear(GL_STENCIL_BUFFER_BIT); 
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE); 


		glUseProgram(current_shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_target.PositionMap);

		renderQuad();

		glClear(GL_DEPTH_BUFFER_BIT);

        current_shader = m_shader_map["deffered"];

		glUseProgram(current_shader);

		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, render_target.AlbedoMap);

		glUniform1i(glGetUniformLocation(current_shader, "NormalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, render_target.NormalMap);

		glUniform1i(glGetUniformLocation(current_shader, "PositionMap"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, render_target.PositionMap);


		GLuint light_dir  = glGetUniformLocation(current_shader, "LightDir");
		glUniform3fv(light_dir, 1, glm::value_ptr(light_dir_vector));

		GLuint view_pos  = glGetUniformLocation(current_shader, "viewPos");
		glUniform3fv(view_pos, 1, glm::value_ptr(Camera.m_position));

        
        GLuint light_color  = glGetUniformLocation(current_shader, "LightColor");
        glUniform3fv(light_color, 1, glm::value_ptr(light_color_vector));

        //glEnable(GL_STENCIL_TEST);
        //glClear(GL_STENCIL_BUFFER_BIT); 
        glStencilMask(0xFF);
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); 

        GLuint ligh_loc  = glGetUniformLocation(current_shader, "lightSpaceMatrix");
        glUniform1i(glGetUniformLocation(current_shader, "shadowMap"), 3);

        DrawGlobalLight(ligh_loc,Light);
        DrawGlobalLight(ligh_loc,Light2);

        glDisable(GL_STENCIL_TEST); 


        
        DrawLight(glm::vec4(hero_position[0],hero_position[1],hero_position[2],0.0f),glm::vec3(0.98f,0.1f,0.1f),render_target);
        

    /**/


        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	   //glEnable(GL_MULTISAMPLE);

		glViewport(0, 0, width, height);

		glClearColor(1.0f, 0.4f, 0.4f, 1.0f);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {// sky
            
            current_shader = m_shader_map["skybox"];
    		glUseProgram(current_shader);
            glm::mat4 model_m = glm::inverse(Camera.CameraMatrix());
            cameraLoc  = glGetUniformLocation(current_shader, "camera");
		    //glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Camera.CameraMatrix()));
		    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(model_m));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.get()->m_texture);
            renderQuad();
            

        }
        glClear(GL_DEPTH_BUFFER_BIT);
        
		current_shader = m_shader_map[m_antialiase_enabled?"sobel_aa":"sobel"];

		glUseProgram(current_shader);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_target.AlbedoMap);

		glUniform1i(glGetUniformLocation(current_shader, "NormalMap"), 1);
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, render_target.NormalMap);


		glUniform1i(glGetUniformLocation(current_shader, "DepthMap"), 2);
		glActiveTexture(GL_TEXTURE0+2);
		glBindTexture(GL_TEXTURE_2D, render_target.depthMap);

		glUniform1i(glGetUniformLocation(current_shader, "LightMap"), 3);
		glActiveTexture(GL_TEXTURE0+3);
		glBindTexture(GL_TEXTURE_2D, final_render_target.AlbedoMap);

        glUniform1i(glGetUniformLocation(current_shader, "SpecMap"), 4);
		glActiveTexture(GL_TEXTURE0+4);
		glBindTexture(GL_TEXTURE_2D, final_render_target.NormalMap);

        renderQuad();/**/

        Draw2D(render_target.depthMap);

}

glm::vec3 IntersectionProjection(const glm::vec3 & position_cube, const glm::vec3 & position_circle, float radius)
{
    glm::vec3 intersection = position_circle - position_cube;
    glm::vec3 return_value = glm::vec3(0.0f,0.0f,0.0f);
    float minimum = std::numeric_limits<float>::min();
    
    float intersect_x = CollisionOnAxe(position_cube[0] -1.0f,
                                        position_cube[0] + 1.0f,
                                        position_circle[0]  - radius,
                                        position_circle[0]  + radius
                                        );

    if(intersect_x < minimum) return glm::vec3(0.0f,0.0f,0.0f);
    float intersect = intersect_x;

    return_value = glm::vec3(intersection[0] > 0 ? intersect_x : -intersect_x,0.0f,0.0f);

    float intersect_z = CollisionOnAxe( position_cube[2] -1.0f,
                                        position_cube[2] + 1.0f,
                                        position_circle[2]  - radius,
                                        position_circle[2]  + radius
                                        );

    if(intersect_z < minimum) return glm::vec3(0.0f,0.0f,0.0f);
    if(intersect_z < intersect)
    {
        intersect = intersect_z;
        return_value = glm::vec3(0.0f,0.0f,intersection[2] > 0 ? intersect_z : -intersect_z);
    }
    glm::vec3 axe = glm::normalize(glm::vec3(1.0f,0.0f,1.0f));

    float pos2_axe = glm::dot(position_circle - position_cube,axe);

    float intersect_xz = CollisionOnAxe( -1.414f,
                                        + 1.414f,
                                        pos2_axe  - radius,
                                        pos2_axe  + radius
                                        );

    if(intersect_xz < minimum) return glm::vec3(0.0f,0.0f,0.0f);
    if(intersect_xz<intersect)
    {
        if(pos2_axe < 0 ) 
            intersect_xz = -intersect_xz;

        return_value = glm::vec3(intersect_xz,0.0f,intersect_xz);
    }
    return return_value;
}


float GlGameStateDungeon::FitObjectToMap(IGlModel& object, glm::vec3 & position)
{
    if(object.mass_inv < 0.001)
        return 0.0f;
    float hero_radius = object.radius;

    int x = static_cast<int>(position[0]*0.5f);
    int z = static_cast<int>(position[2]*0.5f);

    int xp = x;
    int zp = z;

    for(int ix = -1; ix<2; ix++)
    {
        for(int iz = -1; iz<2; iz++)
        {
            xp = x +ix;
            zp = z +iz;
            if(m_dungeon.GetMapObjectIndex(xp,zp,0)>0||m_dungeon.GetMapTilesIndex(xp,zp,0)<0)
            {
                glm::vec3 tile_position = glm::vec3(2.0f * xp,0.0f,2.0f * zp);
                glm::vec3 intersection =IntersectionProjection(tile_position, position, hero_radius);
                position += intersection;
            }
        }
    }

    return 0.0f;
}

float GlGameStateDungeon::FitObjectToObject(IGlModel& object1,IGlModel& object2)
{
    float mass_summ = object1.mass_inv + object2.mass_inv;

    if(mass_summ < std::numeric_limits<float>::min())
            return 0.0f;

    std::pair<float,glm::vec3> intersection = Physics::Intersection(object1,object2);
    
    if (intersection.first < std::numeric_limits<float>::min()) 
        return 0.0f;

    float pos2_axe = glm::dot(object2.position - object1.position,intersection.second);
    intersection.second[1] = 0.0f;
    if(pos2_axe < 0.0f)
    {
        intersection.first = -intersection.first;
    }

    float obj1_part = object1.mass_inv/mass_summ;
    float obj2_part = 1.0f - obj1_part;


    object2.position += obj2_part * intersection.first * intersection.second;
    object1.position -= obj1_part * intersection.first * intersection.second;
    
    return intersection.first;
    
}

InteractionResult GlGameStateDungeon::ReactObjectToEvent(IGlModel& object,IMapEvent& event,std::string &return_value)
{
    std::pair<float,glm::vec3> intersection = Physics::Intersection(object,event);
    return intersection.first < std::numeric_limits<float>::min() ? InteractionResult::Nothing : event.Interact(object,return_value);
}

void GlGameStateDungeon::FitObjects(int steps, float accuracy)
{
    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));
    hero.position = hero_position;

    for(int i =0; i< steps; i++)
    {
        for(auto object : dungeon_objects)
        {  
            auto ptr = object.get();
            FitObjectToObject(*ptr,hero);
            
        }

        FitObjectToMap(hero,hero.position);

        for(auto object : dungeon_objects)
        {  
            auto ptr = object.get();
            FitObjectToMap(*ptr,ptr->position);
            
        }

        for(auto object1 : dungeon_objects)
        {  
            auto ptr1 = object1.get();

            for(auto object2 : dungeon_objects)
            {  
                auto ptr2 = object2.get();

                if(ptr1!=ptr2)
                    FitObjectToObject(*ptr1,*ptr2);
                
            }
            
        }
        
    }
    hero_position = hero.position;

}



void GlGameStateDungeon::MoveHero(const glm::vec3 & hero_move)
{
    hero_position += 1.0f*hero_move;
}

bool IsKilled (std::shared_ptr<IMapEvent> value) { return value.get()->Process() == EventProcessResult::Kill; }


bool GlGameStateDungeon::MobKilled(std::shared_ptr<IGlModel> obj)
{
    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>(m_render_target_map["base_deffered"].get()));
    
    IGlModel * o_ptr = obj.get();
    std::string event_return_string;
    for(auto event : map_events)
    {
        ReactObjectToEvent(*o_ptr,*event.get(),event_return_string);
    }

    for(auto event : mob_events)
    {
        ReactObjectToEvent(*o_ptr,*event.get(),event_return_string);
    }

    if (o_ptr->GetLifeValue() < 0.0f)
        {
            std::shared_ptr<MapEventValhalla>e_ptr(new MapEventValhalla(m_shader_map["sprite2d"],render_target.depthMap,&(fx_texture.get()->m_texture),1.0f,1.4f));
                        
                        MapEventValhalla & event = *(e_ptr.get());
                        
                        event.position = o_ptr->position;
                        event.position.y = 1.5f;
                        map_events.push_back(e_ptr);

            return true;
        }
    return false;
}

void GlGameStateDungeon::MapObjectsEventsInteract()
{
    dungeon_objects.remove_if([this](std::shared_ptr<IGlModel> obj){return MobKilled(obj);});

    mob_events.remove_if(IsKilled);
    map_events.remove_if(IsKilled);
}

void GlGameStateDungeon::ProcessMessage(std::string event_string)
{
    std::map<std::string,const std::function<void(std::stringstream&)>> execute_funcs;

    execute_funcs.insert(std::make_pair("teleport",[this](std::stringstream &sstream)
                                    {
                                        std::string level;
                                        std::string start;
                                        sstream >> level >> start;

                                        LoadMap(level,start);
                                    }));
    std::string info;                               
    std::stringstream ss(event_string);
    ss >> info;
    //size_t first = info.find_first_not_of(' ');
    //size_t last = info.find_last_not_of(' ');
    //std::cout<<"prefix:"<<info<<"\n";
    //std::cout<<"source:"<<event_string<<"\n";
    //std::cout<<"trim prefix: "<<info.substr(first, (last-first+1))<<"\n";
    //str.erase(std::remove(info.begin(), info.end(), ' '), info.end());
    try
        {
            execute_funcs.at(info)(ss);
        }
        catch(const std::out_of_range& exp)
        {
            std::cout<<"Unknown prefix: "<<info<<"\n";
        }

}


bool GlGameStateDungeon::HeroEventsInteract(std::shared_ptr<IGlModel> hero_ptr)
{
    std::string event_return_string;
    for(auto event : hero_events)
    {
       if(ReactObjectToEvent(*hero_ptr,*event.get(),event_return_string) == InteractionResult::PostMessage)
       {
           std::cout<<"\n"<<event_return_string<<"\n"<< hero_ptr->position<<"\n";
           ProcessMessage(event_return_string);
           return true;
       }
    }
    return false;
}

IGlGameState *  GlGameStateDungeon::Process(std::map <int, bool> &inputs, float joy_x, float joy_y)
{
    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>(m_render_target_map["base_deffered"].get()));
    
    static float camera_rotation_angle = 0.0f;
    static float hero_rotation_angle = 0.0f;
    static float old_joy_x = 0.0f;
    std::shared_ptr<IGlModel> hero_ptr = m_models_map["Hero"];
   
    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(hero_ptr.get()));
    GLuint current_shader;

    int models_count = Models.size();
    double time_now = glfwGetTime();
    //std::cout<<(time_now - time)<<'\n';
    if((time_now - time)>(1.0/30.0))
    {
        MapObjectsEventsInteract();
        hero.position = hero_position;
        if(HeroEventsInteract(hero_ptr)) 
        {
           
            hero.position = glm::vec3(0.0f,0.0f,0.0f);
            return this;
        }
        hero.position = glm::vec3(0.0f,0.0f,0.0f);
        

        m_antialiase_enabled = !inputs[GLFW_KEY_F1];
        static float distance = 12.f;
        bool moving = inputs[GLFW_KEY_RIGHT]|inputs[GLFW_KEY_DOWN]|inputs[GLFW_KEY_LEFT]|inputs[GLFW_KEY_UP];

        //key_angle = 0.0f;
        int joy_axes_count;
        const float* joy_axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &joy_axes_count);       
        if(joy_axes!=nullptr)
        {
            if(std::abs(joy_axes[0])+std::abs(joy_axes[1])>0.6f)
            {
                moving = true;
            }
            
            
        }

        if(moving)
        {
            glm::vec3 y_basis = glm::vec3(0.0f,1.0f,0.0f);

            glm::vec3 x_basis = glm::vec3(0.0f,0.0f,0.0f);

            if(joy_axes!=nullptr)
            {
                x_basis[0]= -joy_axes[0];
                x_basis[2]= -joy_axes[1];
            }
            else
            {
                if(inputs[GLFW_KEY_UP])
                {
                    x_basis[2]=1.0f;
                }
                else
                if(inputs[GLFW_KEY_DOWN])
                {
                    x_basis[2]=-1.0f;                            
                }

                if(inputs[GLFW_KEY_LEFT])
                {
                    x_basis[0]=1.0f;
                }
                else
                if(inputs[GLFW_KEY_RIGHT])
                {
                    x_basis[0]=-1.0f;                          
                }
            }

            x_basis = glm::normalize(x_basis);
            
            glm::mat4 m = glm::rotate(glm::radians(camera_rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::vec4 x_basis4 = m *glm::vec4(x_basis[0],x_basis[1],x_basis[2],0.0f);
            x_basis = glm::vec3(x_basis4[0],x_basis4[1],x_basis4[2]);
            x_basis = glm::normalize(x_basis);
            
            glm::vec4 move_h = hero.model_matrix * glm::vec4(1.0f,0.0f,0.0f,1.0f);
            glm::vec3 old_dir = glm::vec3(move_h);

            float l = 0.2f * glm::length(old_dir - x_basis);
            x_basis =(1.0f - l) * old_dir + l * x_basis;
            x_basis = glm::normalize(x_basis);

            glm::vec3 z_basis = glm::cross(x_basis, y_basis);

            glm::mat4 rm(
                glm::vec4(x_basis[0],x_basis[1],x_basis[2],0.0f),
                glm::vec4(y_basis[0],y_basis[1],y_basis[2],0.0f),
                glm::vec4(z_basis[0],z_basis[1],z_basis[2],0.0f),
                glm::vec4(0.0,0.0,0.0,1.0f)
                );

            //hero.model_matrix = glm::mat4();
            //hero.model_matrix = glm::rotate(hero.model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            hero.model_matrix = rm * glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));;
            
        }
        bool attack = inputs[GLFW_MOUSE_BUTTON_LEFT]|inputs[GLFW_KEY_SPACE];

        int buttons_count;
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
        
        if(buttons!= nullptr)
        {
            if(buttons_count>7)
            {
                attack = buttons[7] == GLFW_PRESS;
            }
        }


        if(inputs[GLFW_KEY_RIGHT_BRACKET]) distance +=0.1f;
        if(inputs[GLFW_KEY_LEFT_BRACKET]) distance -=0.1f;

        if(distance<3.0f)distance=3.0f;
        if(distance>14.0f)distance=14.0f;


        float joy_diff = joy_x - old_joy_x;
        if(std::abs(joy_diff) <  0.01f)
        {
            joy_diff = 0.0f;
        }

        if(joy_axes_count>2&&joy_axes!=nullptr)
        {
            joy_diff = joy_axes[2];
        }

        old_joy_x = joy_x;
        camera_rotation_angle -= joy_diff * 12.0f;


        if(camera_rotation_angle > 360.0f)
        {
            camera_rotation_angle -=  360.0f;
        }
        if(camera_rotation_angle < 0.0f)
        {
            camera_rotation_angle +=  360.0f;
        }
        
        glm::vec3 camera_position = glm::vec3(-distance * glm::cos(glm::radians(camera_rotation_angle)), distance,  distance * glm::sin(glm::radians(camera_rotation_angle)));

        Camera.SetCameraLocation(camera_position,glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::vec3 light_orientation = glm::normalize(glm::vec3(-camera_position.x,0.0f,-camera_position.z));
        Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), light_orientation);
        Light2.SetCameraLocation(light_position+light_orientation*10.0f,light_orientation*10.0f, light_orientation);


        time = time_now;

        if(moving&&!attack)
        {
            hero.UseSequence("walk");
            glm::vec4 move_h = hero.model_matrix * glm::vec4(0.0f,0.071f,0.0f,1.0f);
            MoveHero(glm::vec3(move_h));
        }else
        if(attack)
        {
            hero.UseSequence("strike");
            hero.position = hero_position;
            
            mob_events.push_back(AddStrike(hero,render_target));
            hero.position = glm::vec3(0.0f,0.0f,0.0f);
            
        }
        else
        {
            hero.UseSequence("stance");
        }

        hero.Process();
        FitObjects(10,0.0f);
    }
    return this;
}
