#ifndef GL_I_GL_GAME_STATE_DUNGEON
#define GL_I_GL_GAME_STATE_DUNGEON
//#define GLEW_STATIC
//#include <GL/glew.h>
#include "i_gl_game_state.h"
#include "gl_light.h"
#include "gl_render_target.h"
#include "gl_model.h"
#include "gl_character.h"
#include "gl_dungeon.h"

class GlGameStateDungeon: public IGlGameState
{
public:
    GlGameStateDungeon(std::map<std::string,GLuint> &shader_map,
        std::map<std::string,std::shared_ptr<glRenderTarget>> &render_target_map,
        std::map<std::string,std::shared_ptr<IGlModel>> &models_map,
        GLResourcesManager &resources_manager,
        size_t screen_width,
        size_t screen_height);

    ~GlGameStateDungeon()
    {
        
    }
    void Draw();
    IGlGameState * Process(std::map <int, bool> &inputs, float joy_x, float joy_y);
    void SwitchIn(){}
    void SwitchOut(){}
private:
    std::vector <std::shared_ptr<glModel> > Models;
    std::vector <std::shared_ptr<Animation> > Animations;
    std::map<std::string,std::shared_ptr<glRenderTarget>> &m_render_target_map;
    std::map<std::string,std::shared_ptr<IGlModel>> & m_models_map;
    std::shared_ptr<GLuint> sky_texture, fx_texture;
    GlDungeon m_dungeon;
    std::map<std::string,std::shared_ptr<IGlModel>>  dungeon_objects;

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
    
    void DrawDungeon(GLuint current_shader);
    void DrawLight(const glm::vec4 &light_pos_vector,const glm::vec3 &light_color_vector,GLuint current_shader,glRenderTargetDeffered &render_target );
    void MoveHero(const glm::vec3 & hero_move);
    void FitObjects(int steps, float accuracy);
    float FitObjectToMap(IGlModel& object, glm::vec3 & position);
    float FitObjectToObject(IGlModel& object1,IGlModel& object2);
    void DrawFxSprite(GLuint &current_shader, GLuint texture);
};

#endif
