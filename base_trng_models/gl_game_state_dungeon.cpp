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

GlGameStateDungeon::GlGameStateDungeon(std::map<std::string,GLuint> &shader_map,
                                    std::map<std::string,std::shared_ptr<glRenderTarget>> & render_target_map,
                                    std::map<std::string,std::shared_ptr<IGlModel>> & models_map,
                                    size_t screen_width,
                                    size_t screen_height):
                                                        IGlGameState(shader_map,screen_width,screen_height)
                                                        ,m_render_target_map(render_target_map)
                                                        ,m_models_map(models_map)
                                                        ,light_angle (90.0f)
                                                        ,light_radius (20.0f)
                                                        ,now_frame(91)
                                                        ,key_angle(0.0f)
{
    Models.emplace_back(std::make_shared<glModel>("material/tiles/tile.mdl", Animations));
    Models.emplace_back(std::make_shared<glModel>("material/dungeon/statue/statue.mdl", Animations));
    //Models[0]->model = glm::translate(Models[0]->model, glm::vec3(0.0f, 0.92f, 0.0f));
    int models_count = Models.size();
    for(auto tmpModel : Models)
    {
        tmpModel->model = glm::rotate(tmpModel->model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        tmpModel->model = glm::rotate(tmpModel->model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    Camera.SetCameraLocation(glm::vec3(12.0f, 8.485f, -12.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Camera.SetCameraLens(45,(float)screen_width / (float)screen_height,0.1f, 100.0f);

    light_position = glm::vec3(0.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle)));
    light_dir_vector = glm::normalize(light_position);
    Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    float f_near = 1.f;
    float f_far = 35.0f;
    Light.SetCameraLens_Orto(-20.0f, 20.0f,-20.0f, 20.0f,f_near,f_far);

    LoadTexture("material/sky.png",sky_texture);
    time = glfwGetTime();/**/
    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));
    hero.UseSequence("stance");

    m_dungeon_width = 5;
    m_dungeon_height = 5;
    m_dungeon_floors = 1;
    m_dungeon_map_objects.resize(m_dungeon_width*m_dungeon_height*m_dungeon_floors,0);
    m_dungeon_map_objects[1] = 1;
    m_dungeon_map_tiles.resize(m_dungeon_width*m_dungeon_height*m_dungeon_floors,0);
    hero_position = glm::vec3(5.0f,0.0f,5.0f);
}
void GlGameStateDungeon::DrawDungeon(GLuint current_shader)
{
    glm::mat4 model_matrix = Models[0]->model;
    glm::mat4 pos_matrix;
    size_t iz = 0;
    for(int iy = 0; iy < m_dungeon_height; iy++)
    {
        pos_matrix = glm::mat4();
        pos_matrix = glm::translate(pos_matrix, glm::vec3(0.0f, 0.0f, 2.0f*iy) - hero_position);

        for(int ix = 0; ix < m_dungeon_width; ix++)
        {
            int index = m_dungeon_map_tiles[iz*m_dungeon_width*m_dungeon_height + m_dungeon_width*iy +ix];
            pos_matrix = glm::translate(pos_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
            if(index>=0)
            {

                model_matrix = Models[index]->model;
                Models[index]->model = pos_matrix * model_matrix;
                //pos_matrix = glm::translate(pos_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
                Models[index]->Draw(current_shader,now_frame);
                Models[index]->model = model_matrix;
            }
            
            index = m_dungeon_map_objects[iz*m_dungeon_width*m_dungeon_height + m_dungeon_width*iy +ix];
            if(index>0)
            {

                model_matrix = Models[index]->model;
                Models[index]->model = pos_matrix * model_matrix;
                //pos_matrix = glm::translate(pos_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
                Models[index]->Draw(current_shader,now_frame);
                Models[index]->model = model_matrix;
            }
        }
    }/**/
    //Models[index]->model = model_matrix;
    
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

        //pos_matrix = glm::mat4();
        //pos_matrix = glm::translate(pos_matrix, hero_position);
        //model_matrix = hero.model_matrix;
        //hero.model_matrix = pos_matrix * model_matrix;
        //hero.RefreshMatrixes();
		hero.Draw(current_shader);
        //hero.model_matrix = model_matrix;

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

        //model_matrix = hero.model_matrix;
        //pos_matrix = glm::mat4();
        //pos_matrix = glm::translate(pos_matrix, hero_position);

        //hero.model_matrix = pos_matrix * model_matrix;
        //hero.Process();
        hero.Draw(current_shader);
        //hero.model_matrix = model_matrix;






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
            glBindTexture(GL_TEXTURE_2D, sky_texture);

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


}
IGlGameState *  GlGameStateDungeon::Process(std::map <int, bool> &inputs)
{

    GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_models_map["Hero"].get()));;
    GLuint current_shader;

            int models_count = Models.size();
            double time_now = glfwGetTime();
            std::cout<<(time_now - time)<<'\n';
            if((time_now - time)>(1.0/15.0))
                {
                    static float distance = 12.f;

                    if(inputs[GLFW_KEY_RIGHT]) key_angle -=5.0f;
                    if(inputs[GLFW_KEY_LEFT]) key_angle +=5.0f;

                    if(inputs[GLFW_KEY_UP]&&!inputs[GLFW_KEY_SPACE])
                    {
                        hero.UseSequence("walk");//light_angle +=2.0f;
                        glm::vec4 move_h = hero.model_matrix * glm::vec4(0.0f,0.1f,0.0f,1.0f);
                        hero_position += glm::vec3(move_h);//move.xyz();//glm::vec3(move);
                    }else
                    if(inputs[GLFW_KEY_SPACE])
                    {
                        hero.UseSequence("strike");
                    }
                    else
                    {
                        hero.UseSequence("stance");
                    }

                    if(inputs[GLFW_KEY_DOWN]) light_angle -=2.0f;

                    if(inputs[GLFW_KEY_RIGHT_BRACKET]) distance +=0.1f;
                    if(inputs[GLFW_KEY_LEFT_BRACKET]) distance -=0.1f;

                    if(distance<1.0f)distance=1.0f;
                    if(distance>14.0f)distance=14.0f;

                    if(light_angle<10.0f)light_angle=10.0f;
                    if(light_angle>170.0f)light_angle=170.0f;



                    //Camera.SetCameraLocation(glm::vec3(distance, 2.0f, 0.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    Camera.SetCameraLocation(glm::vec3(distance, distance, -distance),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                    glm::vec3 light_position = glm::vec3(7.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle)));
                    Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                    glm::vec3  light_dir_vector = glm::normalize(light_position);
                    time = time_now;
                    //hero.model_matrix = glm::translate(hero.model_matrix, glm::vec3(0.0f, 0.0f, 1.0f));
                    hero.model_matrix = glm::rotate(hero.model_matrix, glm::radians(key_angle), glm::vec3(0.0f, 0.0f, 1.0f));


                    key_angle = 0;
                    now_frame++;
                    if(now_frame == 99 + 1) now_frame = 91;
                    hero.Process();
                }
                return this;
/**/

}
