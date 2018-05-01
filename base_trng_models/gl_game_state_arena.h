#ifndef GL_I_GL_GAME_STATE_ARENA
#define GL_I_GL_GAME_STATE_ARENA
//#define GLEW_STATIC
//#include <GL/glew.h>
#include "i_gl_game_state.h"
#include "gl_light.h"
#include "gl_render_target.h"
#include "gl_model.h"
#include "gl_character.h"
class GlGameStateArena: public IGlGameState
{
public:
    GlGameStateArena(std::map<std::string,GLuint> &shader_map,
        std::map<std::string,std::shared_ptr<glRenderTarget>> &render_target_map,
        std::map<std::string,std::shared_ptr<IGlModel>> &models_map,
        GLResourcesManager &resources_manager,
        size_t screen_width,
        size_t screen_height);

    ~GlGameStateArena(){}
    void Draw();
    IGlGameState * Process(std::map <int, bool> &inputs, float joy_x, float joy_y);
    void SwitchIn(){}
    void SwitchOut(){}
private:
    std::vector <std::shared_ptr<glModel> > Models;
    std::map<std::string,std::shared_ptr<glRenderTarget>> &m_render_target_map;
    std::map<std::string,std::shared_ptr<IGlModel>> & m_models_map;
    std::shared_ptr<IGlTextureStruct>  sky_texture;
    glLight Light;
    float light_angle;
    float light_radius;
    glCamera Camera;
    int now_frame;
    double time;
    float key_angle;
    glm::vec3 light_position;
    glm::vec3 light_dir_vector;
};

#endif
