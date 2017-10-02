#define GLEW_STATIC
#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "glm/glm.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glresourses.h"
#include "glscene.h"
#include "gl_light.h"
#include "gl_render_target.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;

float key_angle = 0.0f;


std::map <int, bool> inputs;

unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}






int main(int argc, char const *argv[])
{
    bool is_fullscreen = true;

    if(argc > 1) is_fullscreen = false;

    inputs[GLFW_KEY_LEFT] =  false;
	inputs[GLFW_KEY_RIGHT] = false;
	inputs[GLFW_KEY_UP] =  false;
	inputs[GLFW_KEY_DOWN] = false;
    inputs[GLFW_KEY_RIGHT_BRACKET] = false;
    inputs[GLFW_KEY_LEFT_BRACKET] = false;

	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL.
	//Мажорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//multisampling
	//glfwWindowHint(GLFW_SAMPLES, 4);

	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);
    GLFWwindow* window;

    if(is_fullscreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        SCR_WIDTH = mode->width;
    	SCR_HEIGHT = mode->height;
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", monitor, nullptr);
    }
    else
    {
	    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", nullptr, nullptr);
    }

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	glRenderTargetDeffered render_target;
	render_target.InitBuffer(width, height);

	glRenderTarget final_render_target;
	final_render_target.InitBuffer(width, height);
    // Build and compile our shader program

    std::map<std::string,GLuint> m_shader_map;

    GLuint shaderProgram = LoadshaderProgram("shaders/vertex1.vs","shaders/frag1.fs");
	GLuint shaderProgram2 = LoadshaderProgram("shaders/vertex2.vs","shaders/frag2.fs");
	GLuint shader_dbg = LoadshaderProgram("shaders/dbg.vs","shaders/dbg.fs");
	GLuint shader_shadow = LoadshaderProgram("shaders/vert1_sh.vs","shaders/frag1_sh.fs");
	GLuint shader_norm = LoadshaderProgram("shaders/vert_norm.vs","shaders/frag_norm.fs");
	GLuint shader_sprite = LoadshaderProgram("shaders/dbg.vs","shaders/sprite.fs");
	GLuint shader_sobel = LoadshaderProgram("shaders/dbg.vs","shaders/sobel_cross.fs");
	GLuint shader_sky = LoadshaderProgram("shaders/sky.vs","shaders/sky.fs");
	
	
	
	m_shader_map.insert ( std::pair<std::string,GLuint>("sprite", LoadshaderProgram("shaders/dbg.vs","shaders/sprite.fs")) );
	m_shader_map.insert ( std::pair<std::string,GLuint>("deffered",LoadshaderProgram("shaders/dbg.vs","shaders/deffered.fs")) );
    m_shader_map.insert ( std::pair<std::string,GLuint>("deffered_simple",LoadshaderProgram("shaders/dbg.vs","shaders/deff_simple.fs")) );
    m_shader_map.insert ( std::pair<std::string,GLuint>("deff_1st_pass",LoadshaderProgram("shaders/vert_norm.vs","shaders/frag_norm.fs")) );
	m_shader_map.insert ( std::pair<std::string,GLuint>("luminocity",LoadshaderProgram("shaders/dbg.vs","shaders/luminocity.fs")) );

	//GLuint sky_tex = LoadshaderProgram("shaders/sky.vs","shaders/sky.fs");


	std::vector <std::shared_ptr<glModel> > Models;
	std::vector <std::shared_ptr<Animation> > Animations;

	Models.emplace_back(std::make_shared<glModel>("material/scene03/scene.mdl",Animations));
/*
	Models.emplace_back(std::make_shared<glModel>("material/bgirl/base.mdl",Animations));
	Models.emplace_back(std::make_shared<glModel>("material/bgirl/hair.mdl",Animations));
	Models.emplace_back(std::make_shared<glModel>("material/bgirl/eyer.mdl",Animations));
	Models.emplace_back(std::make_shared<glModel>("material/bgirl/eyel.mdl",Animations));/**/

	Models.emplace_back(std::make_shared<glModel>("material/new_brb/barb.mdl",Animations));
	Models.emplace_back(std::make_shared<glModel>("material/new_brb/head.mdl",Animations));


	GLuint sky_texture;
	LoadTexture("material/sky.png",sky_texture);

	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);



	glCamera Camera;
	Camera.SetCameraLocation(glm::vec3(12.0f, 2.0f, 0.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Camera.SetCameraLens(45,(float)SCR_WIDTH / (float)SCR_HEIGHT,0.1f, 100.0f);
	glLight Light;
	float light_angle = 90.0f;
	float light_radius = 20.0f;

	glm::vec3 light_position = glm::vec3(0.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle)));

	Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	float f_near = 1.f;
	float f_far = 35.0f;
	Light.SetCameraLens_Orto(-20.0f, 20.0f,-20.0f, 20.0f,f_near,f_far);
	//Light.SetCameraLens(45,(float)SCR_WIDTH / (float)SCR_HEIGHT,0.1f, 100.0f);
	//float angle = 0.0f;
	Models[0]->model = glm::rotate(Models[0]->model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Models[0]->model = glm::rotate(Models[0]->model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Models[1]->model = glm::translate(Models[1]->model, glm::vec3(0.0f, -0.92f, 0.0f));
	Models[1]->model = glm::rotate(Models[1]->model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//test_model.model = glm::rotate(test_model.model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	double time = glfwGetTime();


	int now_frame = 3;

	//float m_mass[16*31] ={0};
	glm::vec3 light_dir_vector = glm::normalize(light_position);

	while(!glfwWindowShouldClose(window))
	{
		GLuint current_shader;

		int models_count = Models.size();
		double time_now = glfwGetTime();
		std::cout<<(time_now - time)<<'\n';
		if((time_now - time)>(1.0/15.0))
			{
                static float distance = 12.f;

				if(inputs[GLFW_KEY_RIGHT]) key_angle +=5.0f;
				if(inputs[GLFW_KEY_LEFT]) key_angle -=5.0f;

				if(inputs[GLFW_KEY_UP]) light_angle +=2.0f;
				if(inputs[GLFW_KEY_DOWN]) light_angle -=2.0f;

                if(inputs[GLFW_KEY_RIGHT_BRACKET]) distance +=0.1f;
                if(inputs[GLFW_KEY_LEFT_BRACKET]) distance -=0.1f;

                if(distance<1.0f)distance=1.0f;
				if(distance>14.0f)distance=14.0f;

				if(light_angle<10.0f)light_angle=10.0f;
				if(light_angle>170.0f)light_angle=170.0f;



                Camera.SetCameraLocation(glm::vec3(distance, 2.0f, 0.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				glm::vec3 light_position = glm::vec3(7.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle)));
				Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				light_dir_vector = glm::normalize(light_position);
				time = time_now;
				//Models[1]->model = glm::rotate(Models[1]->model, glm::radians(key_angle), glm::vec3(0.0f, 0.0f, 1.0f));
				Models[1]->model = glm::rotate(Models[1]->model, glm::radians(key_angle), glm::vec3(0.0f, 0.0f, 1.0f));


				key_angle = 0;
				now_frame++;
				if(now_frame == Animations[0]->frames.size()) now_frame = 3;

				for(int i = 0; i < models_count; i++)
					if(Models[i]->parent_idx != -1)
					Models[i]-> model = Models[Models[i]->parent_idx+1]->model * Animations[Models[i]->parent_idx+1]->frames[now_frame].bones[Models[i]->parent_bone];
			}


		//dress_model.model = Models[0]->model * Animations[0]->frames[now_frame].bones[8];
		//head_model.model = Models[0]->model * Animations[0]->frames[now_frame].bones[19];

		glDisable(GL_MULTISAMPLE);
		glDisable(GL_CULL_FACE);

		unsigned int cameraLoc;


		Light.SetLigtRender();


                glClear( GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		cameraLoc  = glGetUniformLocation(shaderProgram, "camera");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));


		for(int i = 0; i < models_count; i++) Models[i]->Draw(shaderProgram,now_frame);

		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		//dress_model.Draw(shaderProgram2,animation_3,now_frame);
		//head_model.Draw(shaderProgram2,animation_2,now_frame);

		/**/
		//*------------------------------
		render_target.set();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);



		current_shader = m_shader_map["deff_1st_pass"];
		glUseProgram(current_shader);
		cameraLoc  = glGetUniformLocation(current_shader, "camera");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Camera.CameraMatrix()));


		for(int i = 0; i < models_count; i++) Models[i]->Draw(current_shader,now_frame);





		/*
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_MULTISAMPLE);
		glViewport(0, 0, width, height);


		/**/
		/*
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);
		glUseProgram(shaderProgram2);
		cameraLoc  = glGetUniformLocation(shaderProgram2, "camera");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(Camera.CameraMatrix()));


		for(int i = 0; i < models_count; i++) Models[i]->Draw(shaderProgram2,now_frame);

		/**/


		/*---------------------------------------------*/


		final_render_target.set();
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

		glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, width, height);


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

        glm::vec3 light_color_vector = glm::vec3(0.5f,0.5f,1.0f);
        GLuint light_color  = glGetUniformLocation(current_shader, "LightColor");
        glUniform3fv(light_color, 1, glm::value_ptr(light_color_vector));

		GLuint LightLoc  = glGetUniformLocation(current_shader, "lightSpaceMatrix");
		glUniformMatrix4fv(LightLoc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));

		renderQuad();

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

		/*LightLoc  = glGetUniformLocation(current_shader, "lightSpaceMatrix");
		glUniformMatrix4fv(LightLoc, 1, GL_FALSE, glm::value_ptr(Light.CameraMatrix()));*/

        renderQuad();




        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	   //glEnable(GL_MULTISAMPLE);

		glViewport(0, 0, width, height);

		glClearColor(1.0f, 0.4f, 0.4f, 1.0f);
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		current_shader = shader_sobel;

		glUseProgram(current_shader);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_target.AlbedoMap);

		glUniform1i(glGetUniformLocation(current_shader, "NormalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, render_target.NormalMap);


		glUniform1i(glGetUniformLocation(current_shader, "DepthMap"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, render_target.depthMap);

		glUniform1i(glGetUniformLocation(current_shader, "LightMap"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, final_render_target.AlbedoMap);

        renderQuad();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	/*if (action == GLFW_RELEASE)
		return;*/

	if (key == GLFW_KEY_LEFT )
        inputs[GLFW_KEY_LEFT] = (action != GLFW_RELEASE) ?  true : false;

	if (key == GLFW_KEY_RIGHT )
		inputs[GLFW_KEY_RIGHT] = (action != GLFW_RELEASE) ?  true : false;

	if (key == GLFW_KEY_UP )
        inputs[GLFW_KEY_UP] = (action != GLFW_RELEASE) ?  true : false;

	if (key == GLFW_KEY_DOWN )
		inputs[GLFW_KEY_DOWN] = (action != GLFW_RELEASE) ?  true : false;

    if (key == GLFW_KEY_LEFT_BRACKET )
        inputs[GLFW_KEY_LEFT_BRACKET] = (action != GLFW_RELEASE) ?  true : false;

    if (key == GLFW_KEY_RIGHT_BRACKET )
        inputs[GLFW_KEY_RIGHT_BRACKET] = (action != GLFW_RELEASE) ?  true : false;

}
