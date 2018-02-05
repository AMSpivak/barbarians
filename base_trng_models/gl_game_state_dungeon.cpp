#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include  <functional>
//#define GLM_SWIZZLE_XYZW

#include "glm/glm.hpp"

#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glscene.h"
#include "gl_game_state_dungeon.h"
#include "map_event_hero_strikes.h"
#include "map_event_valhalla.h"
#include "collision.h"

void LoadLineBlock(std::ifstream &file,const std::string &sufix,const std::function<void(const std::string &)> process)
{


    std::string sufix_start("<"+sufix+">");
    std::string sufix_end("<!"+sufix+">");
    std::string tempholder("");

    while(!file.eof()&&(tempholder.compare(sufix_start)))
    {
        getline(file, tempholder);
        std::cout<<tempholder<<"\n";
    }


    bool is_block_endline = false;

    while(!file.eof()&&(!is_block_endline))
    {
        getline(file, tempholder);
        is_block_endline = tempholder.compare(sufix_end) == 0;
        if(!is_block_endline)
        {
            process(tempholder);
        }
    }
    

}

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
                                                        ,light_angle (90.0f)
                                                        ,light_radius (20.0f)
                                                        ,now_frame(91)
                                                        ,key_angle(0.0f)
                                                        ,m_dungeon(10,10,1)
{

    Camera.SetCameraLocation(glm::vec3(12.0f, 8.485f, -12.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Camera.SetCameraLens(45,(float)screen_width / (float)screen_height,0.1f, 100.0f);
  

    //debug_texture = resources_manager.m_texture_atlas.Assign("fireball.png");
    
    time = glfwGetTime();/**/
    LoadMap("levels/test.lvl");
}




void GlGameStateDungeon::LoadMap(const std::string &filename)
{


    GLResourcesManager * resources_manager = GetResourceManager();
    
	std::ifstream level_file;
	level_file.open(filename); 
    
    std::cout<<"Level:"<<filename<<" "<<(level_file.is_open()?"-opened":"-failed")<<"\n";  
    light_radius = 10.0f;
    //light_position = glm::vec3(0.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle))); 
    light_position = glm::vec3(light_radius, 2 * light_radius/*glm::sin(glm::radians(light_angle))*/, -light_radius/*glm::cos(glm::radians(light_angle))*/);
    
    light_dir_vector = glm::normalize(light_position);
    Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    float f_near = 1.f;
    float f_far = 35.0f;
    Light.SetCameraLens_Orto(-20.0f, 20.0f,-20.0f, 20.0f,f_near,f_far);


    LoadLineBlock(level_file,"models",[this](const std::string &line)
                                        {
                                            Models.emplace_back(std::make_shared<glModel>(line, Animations));
                                        }
                                        );
    ResetModels(Models);

    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));
    hero.UseSequence("stance");

    hero_position = glm::vec3(10.0f,0.0f,10.0f);  

    //sky_texture = resources_manager->m_texture_atlas.Assign("dungeon_bck.png");
    skybox = resources_manager->m_texture_atlas.Assign("skybox/violent.cub");
    fx_texture = resources_manager->m_texture_atlas.Assign("valh.png");

    {
        std::shared_ptr<IGlModel> barrel_ptr(new GlCharacter());
        dungeon_objects.push_back( barrel_ptr);
        GlCharacter & barrel_model =  *(dynamic_cast<GlCharacter*>(barrel_ptr.get()));
        barrel_model.mass_inv = 1.0f;
        barrel_model.radius = 0.5f;
        barrel_model.position = glm::vec3(10.0f,0.0f,12.0f);
        barrel_model.model_matrix = glm::rotate(barrel_model.model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        barrel_model.AddModel("material/barrel/barrel.mdl");
        AnimationSequence as_base(0,1);
        barrel_model.AddSequence("base",as_base);
        AnimationSequence as_damage(2,3);
        barrel_model.AddSequence("damage",as_damage);
        barrel_model.UseSequence("base");
    }

    {
        std::shared_ptr<IGlModel> barrel_ptr(new GlCharacter());
        dungeon_objects.push_back( barrel_ptr);
        GlCharacter & barrel_model =  *(dynamic_cast<GlCharacter*>(barrel_ptr.get()));
        barrel_model.mass_inv = 1.0f;
        barrel_model.radius = 0.5f;
        barrel_model.position = glm::vec3(12.0f,0.0f,12.0f);
        barrel_model.model_matrix = glm::rotate(barrel_model.model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        barrel_model.AddModel("material/barrel/barrel.mdl");
        AnimationSequence as_base(0,1);
        AnimationSequence as_damage(2,3);
        barrel_model.AddSequence("damage",as_damage);
        barrel_model.AddSequence("base",as_base);
        barrel_model.UseSequence("base");
    }
    level_file.close();  

}
void GlGameStateDungeon::DrawDungeon(GLuint current_shader)
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
                //pos_matrix = glm::translate(pos_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
                Models[index]->Draw(current_shader,now_frame);
                Models[index]->model = model_matrix;
            }
            
            index = m_dungeon.GetMapObjectIndex(ix,iy,iz);//m_dungeon_map_objects[iz*m_dungeon_width*m_dungeon_height + m_dungeon_width*iy +ix];
            if(index>0)
            {

                model_matrix = Models[index]->model;
                Models[index]->model = pos_matrix * model_matrix;
                //pos_matrix = glm::translate(pos_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
                Models[index]->Draw(current_shader,now_frame);
                Models[index]->model = model_matrix;
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
               
    
}

void DrawSimpleLight(const glm::vec4 &light_pos_vector,const glm::vec3 &light_color_vector,GLuint current_shader,glRenderTargetDeffered &render_target)
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


		GLuint light_pos  = glGetUniformLocation(current_shader, "LightLocation");
		glUniform4fv(light_pos, 1, glm::value_ptr(light_pos_vector));

        
        GLuint light_color  = glGetUniformLocation(current_shader, "LightColor");
        glUniform3fv(light_color, 1, glm::value_ptr(light_color_vector));

        renderQuad();
}

void GlGameStateDungeon::DrawLight(const glm::vec4 &light_pos_vector,const glm::vec3 &light_color_vector,GLuint current_shader,glRenderTargetDeffered &render_target )
{
        //DrawSimpleLight(light_pos_vector,light_color_vector,current_shader,render_target );
    glm::vec4 light_position;
    glm::vec3 light_color;
    
    for(std::shared_ptr<IMapEvent> event :map_events) 
      {
          IMapEvent * e_ptr = event.get();
          if(e_ptr->IsLight(light_position,light_color))
          {
                DrawSimpleLight(light_position - light_pos_vector,light_color,current_shader,render_target );
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
          IMapEvent * e_ptr = event.get();
          e_ptr->Show(hero_position,Camera);
      }
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

		Light.SetLigtRender();
        glClear( GL_DEPTH_BUFFER_BIT);
		GLuint current_shader = m_shader_map["shadowmap"];
		glUseProgram(current_shader);
		cameraLoc  = glGetUniformLocation(current_shader, "camera");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));

        DrawDungeon(current_shader);
		hero.Draw(current_shader);

		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);


		//*------------------------------
		render_target.set();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);



		current_shader = m_shader_map["deff_1st_pass"];
		glUseProgram(current_shader);
		cameraLoc  = glGetUniformLocation(current_shader, "camera");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Camera.CameraMatrix()));

        DrawDungeon(current_shader);


        hero.Draw(current_shader);
     
		final_render_target.set();
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);


		glViewport(0, 0, width, height);

		glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		current_shader = m_shader_map["luminocity"];

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

		glUniform1i(glGetUniformLocation(current_shader, "shadowMap"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Light.depthMap);


		GLuint light_dir  = glGetUniformLocation(current_shader, "LightDir");
		glUniform3fv(light_dir, 1, glm::value_ptr(light_dir_vector));

		GLuint view_pos  = glGetUniformLocation(current_shader, "viewPos");
		glUniform3fv(view_pos, 1, glm::value_ptr(Camera.m_position));

        glm::vec3 light_color_vector = glm::vec3(1.0f,1.0f,1.0f);
        GLuint light_color  = glGetUniformLocation(current_shader, "LightColor");
        glUniform3fv(light_color, 1, glm::value_ptr(light_color_vector));

		GLuint LightLoc  = glGetUniformLocation(current_shader, "lightSpaceMatrix");
		glUniformMatrix4fv(LightLoc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));

		renderQuad();

        current_shader = m_shader_map["deffered_simple"];

        view_pos  = glGetUniformLocation(current_shader, "viewPos");
		glUniform3fv(view_pos, 1, glm::value_ptr(Camera.m_position));

        DrawLight(glm::vec4(hero_position[0],hero_position[1],hero_position[2],0.0f),glm::vec3(0.98f,0.1f,0.1f),current_shader,render_target);
        

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
            glBindTexture(GL_TEXTURE_CUBE_MAP, *skybox.get());
            renderQuad();
            

        }
        glClear(GL_DEPTH_BUFFER_BIT);
		current_shader = m_shader_map["sobel"];

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

    std::vector < glm::vec3 > axes;
    axes.push_back(glm::normalize(object2.position - object1.position));
    object1.AddAxes(axes);
    object2.AddAxes(axes);

    glm::vec3 compensate_axe(0.0f,0.0f,0.0f);
    float intersection = std::numeric_limits<float>::max();

    for(auto axe : axes)
    {
        std::pair<float,float> projection1 = object1.ProjectOnAxe(axe);
        std::pair<float,float> projection2 = object2.ProjectOnAxe(axe);
       float axe_intersection = CollisionOnAxe(projection1,projection2);

        if(axe_intersection < std::numeric_limits<float>::min())
            return 0.0f;

        if(axe_intersection < intersection)
        {
            compensate_axe = axe;
            intersection = axe_intersection;
        }
    }

    float pos2_axe = glm::dot(object2.position - object1.position,compensate_axe);
    compensate_axe[1] = 0.0f;
    if(pos2_axe < 0.0f)
    {
        intersection = -intersection;
    }

    float obj1_part = object1.mass_inv/mass_summ;
    float obj2_part = 1.0f - obj1_part;


    object2.position += obj2_part * intersection * compensate_axe;
    object1.position -= obj1_part * intersection * compensate_axe;
    
    return intersection;
    
}

InteractionResult GlGameStateDungeon::ReactObjectToEvent(IGlModel& object,IMapEvent& event)
{
    std::vector < glm::vec3 > axes;
    axes.push_back(glm::normalize(object.position - event.position));
    object.AddAxes(axes);
    event.AddAxes(axes);

    glm::vec3 compensate_axe(0.0f,0.0f,0.0f);
    float intersection = std::numeric_limits<float>::max();

    for(auto axe : axes)
    {
        std::pair<float,float> projection1 = object.ProjectOnAxe(axe);
        std::pair<float,float> projection2 = event.ProjectOnAxe(axe);
        float axe_intersection = CollisionOnAxe(projection1,projection2);

        if(axe_intersection < std::numeric_limits<float>::min())
            return InteractionResult::Nothing;

        if(axe_intersection < intersection)
        {
            compensate_axe = axe;
            intersection = axe_intersection;
        }
    }
    
    return event.Interact(object);
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

IGlGameState *  GlGameStateDungeon::Process(std::map <int, bool> &inputs, float joy_x, float joy_y)
{
    std::list<std::shared_ptr<IGlModel>>::iterator cur_obj = dungeon_objects.begin();
    std::list<std::shared_ptr<IGlModel>>::iterator end_obj = dungeon_objects.end();
    
    std::list<std::shared_ptr<IMapEvent>>::iterator cur = map_events.begin();
    std::list<std::shared_ptr<IMapEvent>>::iterator end = map_events.end();

    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>(m_render_target_map["base_deffered"].get()));
    
    
    while (cur_obj != end_obj)
    {
        IGlModel * o_ptr = cur_obj->get();

        while (cur != end)
        {
            IMapEvent * e_ptr = cur->get();
            InteractionResult interaction = ReactObjectToEvent(*o_ptr,*e_ptr);
            ++cur;
        }
        cur = map_events.begin();

        if (cur_obj->get()->GetLifeValue() < 0.0f)
        {
            std::shared_ptr<MapEventValhalla>e_ptr(new MapEventValhalla(m_shader_map["sprite2d"],render_target.depthMap,fx_texture.get(),1.0f,1.4f));
                        
                        MapEventValhalla & event = *(e_ptr.get());
                        
                        event.position = cur_obj->get()->position;
                        event.position.y = 1.5f;
                        map_events.push_back(e_ptr);

            dungeon_objects.erase(cur_obj++); 
        }  
        else
        {
            ++cur_obj;
        }
        
    }
    
    while (cur != end)
    {
        if (cur->get()->Process() == EventProcessResult::Kill)
        {
            map_events.erase(cur++); 
        }  
        else
        {
            ++cur;
        }
    }
    
    



    static float camera_rotation_angle = 0.0f;
    static float hero_rotation_angle = 0.0f;
    static float old_joy_x = 0.0f;
   
    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));;
    GLuint current_shader;

            int models_count = Models.size();
            double time_now = glfwGetTime();
            //std::cout<<(time_now - time)<<'\n';
            if((time_now - time)>(1.0/30.0))
                {
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

                        float l = 0.4f * glm::length(old_dir - x_basis);
                        x_basis =(1.0f - l) * old_dir + l * x_basis;
                        x_basis = glm::normalize(x_basis);

                        glm::vec3 z_basis = glm::cross(x_basis, y_basis);

                        glm::mat4 rm(
                            glm::vec4(x_basis[0],x_basis[1],x_basis[2],0.0f),
                            glm::vec4(y_basis[0],y_basis[1],y_basis[2],0.0f),
                            glm::vec4(z_basis[0],z_basis[1],z_basis[2],0.0f),
                            glm::vec4(0.0,0.0,0.0,1.0f)
                            );

                        hero.model_matrix = glm::mat4();
                        hero.model_matrix = glm::rotate(hero.model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                        hero.model_matrix = rm * hero.model_matrix;
                       
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

                    if(distance<5.0f)distance=5.0f;
                    if(distance>14.0f)distance=14.0f;

                    if(light_angle<10.0f)light_angle=10.0f;
                    if(light_angle>170.0f)light_angle=170.0f;

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
                    //Camera.SetCameraLocation(glm::vec3(distance, 2.0f, 0.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    glm::vec3 camera_position = glm::vec3(-distance * glm::cos(glm::radians(camera_rotation_angle)), distance,  distance * glm::sin(glm::radians(camera_rotation_angle)));

                    Camera.SetCameraLocation(camera_position,glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    //light_radius = 10;
                    //glm::vec3 light_position = glm::vec3(light_radius, 2 * light_radius/*glm::sin(glm::radians(light_angle))*/, -light_radius/*glm::cos(glm::radians(light_angle))*/);
                    //Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    glm::vec3  light_dir_vector = glm::normalize(light_position);
                    time = time_now;
   
                    if(moving&&!attack)
                    {
                        hero.UseSequence("walk");
                        glm::vec4 move_h = hero.model_matrix * glm::vec4(0.0f,0.1f,0.0f,1.0f);
                        MoveHero(glm::vec3(move_h));
                    }else
                    if(attack)
                    {
                        hero.UseSequence("strike");
                        std::shared_ptr<IMapEventHeroStrike>e_ptr(new IMapEventHeroStrike(m_shader_map["sprite2d"],render_target.depthMap,fx_texture.get(),1.0f,1.4f));
                        IMapEventHeroStrike & event = *(e_ptr.get());
                        event.model_matrix = hero.model_matrix;
                        event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(0.3f,0.5f,0.0f),glm::vec3(0.5f,2.5f,0.0f)));
                        event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(0.5f,2.5f,0.0f),glm::vec3(-0.5f,2.5f,0.0f)));
                        event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(-0.5f,2.5f,0.0f),glm::vec3(-0.3f,0.5f,0.0f)));
                        event.AddEdge(std::pair<glm::vec3,glm::vec3>(glm::vec3(-0.3f,0.5f,0.0f),glm::vec3(0.3f,0.5f,0.0f)));
                        event.position = hero.position;
                        map_events.push_back(e_ptr);
                    }
                    else
                    {
                        hero.UseSequence("stance");
                    }

                    now_frame++;
                    if(now_frame == 99 + 1) now_frame = 91;
                    hero.Process();
                    FitObjects(10,0.0f);
                }


                

                return this;
}
