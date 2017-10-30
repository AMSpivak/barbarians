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
#include "gl_render_target.h"

#include "gl_texture_atlas.h"
#include "gl_game_state_spinners.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;

float key_angle = 0.0f;


std::map <int, bool> inputs;


int main(int argc, char const *argv[])
{
    bool is_fullscreen = true;

    if(argc > 1) is_fullscreen = false;

    inputs[GLFW_KEY_LEFT] =  false;
	inputs[GLFW_KEY_RIGHT] = false;
	inputs[GLFW_KEY_UP] =  false;
	inputs[GLFW_KEY_DOWN] = false;
    inputs[GLFW_KEY_SPACE] = false;


	//GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//multisampling
	glfwWindowHint(GLFW_SAMPLES, 4);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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

    std::map<std::string,GLuint> m_shader_map;


	m_shader_map.insert ( std::pair<std::string,GLuint>("sprite", LoadshaderProgram("shaders/sprite.vs","shaders/sprite.fs")) );






	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);


	GLTextureAtlas texture_atlas;
	
	GlGameStateSpinner  game_state_spinner(m_shader_map,texture_atlas,width,height);
	IGlGameState * game_state = &game_state_spinner;


	while(!glfwWindowShouldClose(window))
	{
        game_state->Process(inputs);
		game_state->Draw();
		
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

    if (key == GLFW_KEY_SPACE )
    		inputs[GLFW_KEY_SPACE] = (action != GLFW_RELEASE) ?  true : false;



}
