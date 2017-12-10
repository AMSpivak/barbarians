#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#define GLM_SWIZZLE_XYZW

#include "glm/glm.hpp"

#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glscene.h"
#include "gl_game_state_dungeon.h"
#include "collision.h"

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
    Models.emplace_back(std::make_shared<glModel>("material/tiles/tile.mdl", Animations));
    Models.emplace_back(std::make_shared<glModel>("material/dungeon/statue/statue.mdl", Animations));
    Models.emplace_back(std::make_shared<glModel>("material/dungeon/wall/wall.mdl", Animations));
    Models.emplace_back(std::make_shared<glModel>("material/dungeon/wallcross/wallcross.mdl", Animations));
    Models.emplace_back(std::make_shared<glModel>("material/dungeon/wally/wall.mdl", Animations));
    //Models[0]->model = glm::translate(Models[0]->model, glm::vec3(0.0f, 0.92f, 0.0f));
    int models_count = Models.size();
    for(auto tmpModel : Models)
    {
        tmpModel->model = glm::rotate(tmpModel->model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        tmpModel->model = glm::rotate(tmpModel->model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    Camera.SetCameraLocation(glm::vec3(12.0f, 8.485f, -12.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Camera.SetCameraLens(45,(float)screen_width / (float)screen_height,0.1f, 100.0f);

    //light_position = glm::vec3(0.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle)));
    light_position = glm::vec3(0.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle))); 
    light_dir_vector = glm::normalize(light_position);
    Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    float f_near = 1.f;
    float f_far = 35.0f;
    Light.SetCameraLens_Orto(-20.0f, 20.0f,-20.0f, 20.0f,f_near,f_far);
    sky_texture = resources_manager.m_texture_atlas.Assign("dungeon_bck.png");
    fx_texture = resources_manager.m_texture_atlas.Assign("fireball.png");
    /*LoadTexture("material/dungeon_bck.png",sky_texture);
    LoadTexture("material/fireball.png",fx_texture);*/
    
    time = glfwGetTime();/**/
    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));
    hero.UseSequence("stance");
/*
    m_dungeon_width = 5;
    m_dungeon_height = 5;
    m_dungeon_floors = 1;
    m_dungeon_map_objects.resize(m_dungeon_width*m_dungeon_height*m_dungeon_floors,0);
    m_dungeon_map_objects[1] = 1;
    m_dungeon_map_tiles.resize(m_dungeon_width*m_dungeon_height*m_dungeon_floors,0);*/
    hero_position = glm::vec3(10.0f,0.0f,10.0f);
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
    }/**/
    //Models[index]->model = model_matrix;
    
}

void GlGameStateDungeon::DrawLight(const glm::vec4 &light_pos_vector,const glm::vec3 &light_color_vector,GLuint current_shader,glRenderTargetDeffered &render_target )
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

        glm::vec3 light_color_vector = glm::vec3(1.0f,1.0f,1.0f);
        GLuint light_color  = glGetUniformLocation(current_shader, "LightColor");
        glUniform3fv(light_color, 1, glm::value_ptr(light_color_vector));

		GLuint LightLoc  = glGetUniformLocation(current_shader, "lightSpaceMatrix");
		glUniformMatrix4fv(LightLoc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));

		renderQuad();
/*
        glClear(GL_DEPTH_BUFFER_BIT);
        current_shader = m_shader_map["deffered_simple"];
        glUseProgram(current_shader);

		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, render_target.AlbedoMap);

		glUniform1i(glGetUniformLocation(current_shader, "NormalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, render_target.NormalMap);

		glUniform1i(glGetUniformLocation(current_shader, "PositionMap"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, render_target.PositionMap);

        glm::vec4 light_pos_vector = glm::vec4(0.0f,-0.5f,0.0f,15.0f);

		GLuint light_pos  = glGetUniformLocation(current_shader, "LightLocation");
		glUniform4fv(light_pos, 1, glm::value_ptr(light_pos_vector));

        glm::vec3 light_color_vector2 = glm::vec3(9.0f,5.5f,0.2f);
        light_color  = glGetUniformLocation(current_shader, "LightColor");
        glUniform3fv(light_color, 1, glm::value_ptr(light_color_vector2));

        renderQuad();*/
        //glDisable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	   //glEnable(GL_MULTISAMPLE);

		glViewport(0, 0, width, height);

		glClearColor(1.0f, 0.4f, 0.4f, 1.0f);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            current_shader = m_shader_map["sprite"];
    		glUseProgram(current_shader);

            glm::mat4 model_m = glm::mat4(1.0f);
            model_m = glm::scale(model_m,glm::vec3(1.0f,(float)width/height,1.0f));
            glm::mat4 camera_m = glm::mat4(1.0f);
            cameraLoc  = glGetUniformLocation(current_shader, "camera");
    		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera_m));

            GLuint model_matrix  = glGetUniformLocation(current_shader, "model");
            glUniformMatrix4fv(model_matrix, 1, GL_FALSE, glm::value_ptr(model_m));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *sky_texture.get());

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

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClear(GL_DEPTH_BUFFER_BIT);
        GLuint sprite_shader = 0;
        DrawFxSprite(sprite_shader,*fx_texture.get());
        glDisable(GL_BLEND);

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

    float intersect_z = CollisionOnAxe(position_cube[2] -1.0f,
                                        position_cube[2] + 1.0f,
                                        position_circle[2]  - radius,
                                        position_circle[2]  + radius
                                        );
    if(intersect_z < minimum) return glm::vec3(0.0f,0.0f,0.0f);
    if(intersect_z<intersect)
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
void GlGameStateDungeon::FitObjects(int steps, float accuracy)
{
    float hero_radius =1.0f;

    int x = static_cast<int>(hero_position[0]*0.5f);
    int z = static_cast<int>(hero_position[2]*0.5f);

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
                glm::vec3 intersection =IntersectionProjection(tile_position, hero_position, hero_radius);
                hero_position += intersection;
            }
        }
    }

}


void GlGameStateDungeon::MoveHero(const glm::vec3 & hero_move)
{
    hero_position += hero_move;
}

IGlGameState *  GlGameStateDungeon::Process(std::map <int, bool> &inputs, float joy_x, float joy_y)
{
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
                        //x_basis = glm::normalize(x_basis);
                        x_basis = glm::normalize(x_basis);
                        
                        glm::mat4 m = glm::rotate(glm::radians(camera_rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f));
                        glm::vec4 x_basis4 = m *glm::vec4(x_basis[0],x_basis[1],x_basis[2],0.0f);
                        x_basis = glm::vec3(x_basis4[0],x_basis4[1],x_basis4[2]);
                        x_basis = glm::normalize(x_basis);
                        
                        glm::vec4 move_h = hero.model_matrix * glm::vec4(1.0f,0.0f,0.0f,1.0f);
                        glm::vec3 old_dir = glm::vec3(move_h);

                        float l = 0.4f * glm::length(old_dir - x_basis);
                        x_basis =(1.0f - l) * old_dir + l * x_basis;
                        //x_basis =0.6f * old_dir + 0.4f * x_basis;
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
                    light_radius = 10;
                    glm::vec3 light_position = glm::vec3(light_radius, 2 * light_radius/*glm::sin(glm::radians(light_angle))*/, -light_radius/*glm::cos(glm::radians(light_angle))*/);
                    Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    glm::vec3  light_dir_vector = glm::normalize(light_position);
                    time = time_now;

                    //hero.model_matrix = glm::mat4();
                    //hero.model_matrix = glm::rotate(hero.model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    //hero.model_matrix = glm::rotate(hero.model_matrix, glm::radians(hero_rotation_angle/*key_angle*/), glm::vec3(0.0f, 0.0f, 1.0f));
                    
                    if(moving&&!attack)
                    {
                        hero.UseSequence("walk");
                        glm::vec4 move_h = hero.model_matrix * glm::vec4(0.0f,0.1f,0.0f,1.0f);
                        MoveHero(glm::vec3(move_h));
                    }else
                    if(attack)
                    {
                        hero.UseSequence("strike");
                    }
                    else
                    {
                        hero.UseSequence("stance");
                    }

                    //key_angle = 0;
                    now_frame++;
                    if(now_frame == 99 + 1) now_frame = 91;
                    hero.Process();
                    FitObjects(0,0.0f);
                }
                return this;
/**/

}
