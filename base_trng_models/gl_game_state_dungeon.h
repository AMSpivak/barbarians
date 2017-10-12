#ifndef GL_I_GL_GAME_STATE_DUNGEON
#define GL_I_GL_GAME_STATE_DUNGEON
//#define GLEW_STATIC
//#include <GL/glew.h>
#include "i_gl_game_state.h"
#include "gl_light.h"
#include "gl_render_target.h"
#include "gl_model.h"
#include "gl_character.h"
class GlGameStateDungeon: public IGlGameState
{
public:
    GlGameStateDungeon(std::map<std::string,GLuint> &shader_map,
        std::map<std::string,std::shared_ptr<glRenderTarget>> &render_target_map,
        std::map<std::string,std::shared_ptr<IGlModel>> &models_map,
        size_t screen_width,
        size_t screen_height);

    ~GlGameStateDungeon(){}
    void Draw();
    void Process(std::map <int, bool> &inputs);
private:
    std::vector <std::shared_ptr<glModel> > Models;
    std::vector <std::shared_ptr<Animation> > Animations;
    std::map<std::string,std::shared_ptr<glRenderTarget>> &m_render_target_map;
    std::map<std::string,std::shared_ptr<IGlModel>> & m_models_map;
    GLuint sky_texture;
    glLight Light;
    float light_angle;
    float light_radius;
    glCamera Camera;
    int now_frame;
    double time;
    float key_angle;
    glm::vec3 hero_position;
    glm::vec3 light_position;
    glm::vec3 light_dir_vector;
};

#endif
