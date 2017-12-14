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
#include "gl_resources_manager.h"

#include "glscene.h"
#include "gl_light.h"
#include "gl_render_target.h"
#include "gl_model.h"
#include "gl_character.h"
#include "gl_game_state_arena.h"
#include "gl_game_state_dungeon.h"
#include "animation_sequence.h"
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	

	GLResourcesManager resources_atlas("material/textures/");

	SetResourceManager(&resources_atlas);

    std::map<std::string,GLuint> m_shader_map;

    std::map<std::string,std::shared_ptr<glRenderTarget>> m_render_target_map;
    {
        std::shared_ptr<glRenderTarget> r_target(new glRenderTargetDeffered());
        m_render_target_map.insert( std::pair<std::string,std::shared_ptr<glRenderTarget>>("base_deffered",r_target));
        std::shared_ptr<glRenderTarget> r_target_final(new glRenderTarget());
        m_render_target_map.insert( std::pair<std::string,std::shared_ptr<glRenderTarget>>("final",r_target_final));
    }

    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>(m_render_target_map["base_deffered"].get()));
	glRenderTarget &final_render_target = *(m_render_target_map["final"].get());
	//glRenderTargetDeffered render_target;
	render_target.InitBuffer(width, height);

	//glRenderTarget final_render_target;
	final_render_target.InitBuffer(width, height);
    // Build and compile our shader program


    m_shader_map.insert ( std::pair<std::string,GLuint>("sobel", LoadshaderProgram("shaders/dbg.vs","shaders/sobel_cross.fs")) );
	m_shader_map.insert ( std::pair<std::string,GLuint>("shadowmap", LoadshaderProgram("shaders/vertex1.vs","shaders/frag1.fs")) );
	m_shader_map.insert ( std::pair<std::string,GLuint>("sprite", LoadshaderProgram("shaders/sprite.vs","shaders/sprite.fs")) );
	m_shader_map.insert ( std::pair<std::string,GLuint>("deffered",LoadshaderProgram("shaders/dbg.vs","shaders/deffered.fs")) );
    m_shader_map.insert ( std::pair<std::string,GLuint>("deffered_simple",LoadshaderProgram("shaders/dbg.vs","shaders/deff_simple.fs")) );
    m_shader_map.insert ( std::pair<std::string,GLuint>("deff_1st_pass",LoadshaderProgram("shaders/vert_norm.vs","shaders/frag_norm.fs")) );
	m_shader_map.insert ( std::pair<std::string,GLuint>("luminocity",LoadshaderProgram("shaders/dbg.vs","shaders/luminocity.fs")) );



	std::vector <std::shared_ptr<glModel> > Models;
	std::vector <std::shared_ptr<Animation> > Animations;

	Models.emplace_back(std::make_shared<glModel>("material/scene03/scene.mdl",Animations));

    std::map<std::string,std::shared_ptr<IGlModel>> m_glmodels_map;

    {
        std::shared_ptr<IGlModel> r_model(new GlCharacter());
        m_glmodels_map.insert( std::pair<std::string,std::shared_ptr<IGlModel>>("Hero",r_model));
    }

	GlCharacter &hero =  *(dynamic_cast<GlCharacter*>(m_glmodels_map["Hero"].get()));
	hero.mass_inv = 1.0;
	hero.AddModel("material/new_brb/barb.mdl");
	hero.AddModel("material/new_brb/head.mdl");
	hero.AddModel("material/b_axe/axe.mdl");
    AnimationSequence as_stance(91,99);
    AnimationSequence as_walk(127,135);
    hero.AddSequence("stance",as_stance);
    hero.AddSequence("walk",as_walk);
    AnimationSequence as_strike(149,157);
    hero.AddSequence("strike",as_strike);





	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);




	hero.model_matrix = glm::rotate(hero.model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


   // GlGameStateArena game_state_arena(m_shader_map,m_render_target_map,m_glmodels_map,resources_atlas,width,height);
    GlGameStateDungeon game_state_dungeon(m_shader_map,m_render_target_map,m_glmodels_map,resources_atlas,width,height);
    IGlGameState * game_state = nullptr;//&game_state_arena;
    game_state = &game_state_dungeon;


	while(!glfwWindowShouldClose(window))
	{
		GLuint current_shader;
		double xpos, ypos;

		
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = (xpos * 2.0f - width)/width;
		ypos = (ypos * 2.0f - height)/height;
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        inputs[GLFW_MOUSE_BUTTON_LEFT] = (state != GLFW_RELEASE) ?  true : false;
        
		game_state->Process(inputs, xpos, ypos);
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

    if (key == GLFW_KEY_LEFT_BRACKET )
        inputs[GLFW_KEY_LEFT_BRACKET] = (action != GLFW_RELEASE) ?  true : false;

    if (key == GLFW_KEY_RIGHT_BRACKET )
        inputs[GLFW_KEY_RIGHT_BRACKET] = (action != GLFW_RELEASE) ?  true : false;

}
