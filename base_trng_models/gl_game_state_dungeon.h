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
#include "i_map_event.h"
#include "loader.h"
#include "gl_font16x16.h"
#include <list>

class GlGameStateDungeon: public IGlGameState
{
public:
    GlGameStateDungeon(std::map<const std::string,GLuint> &shader_map,
        std::map<std::string,std::shared_ptr<glRenderTarget>> &render_target_map,
        std::map<std::string,std::shared_ptr<GlCharacter>> &models_map,
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
    std::shared_ptr<IGlText> m_gl_text;


    std::vector <std::shared_ptr<glModel> > Models;
    std::map<std::string,std::shared_ptr<glRenderTarget>> &m_render_target_map;
    std::map<std::string,std::shared_ptr<GlCharacter>> & m_models_map;
    std::shared_ptr<IGlTextureStruct> fx_texture, skybox, debug_texture;
    GlDungeon m_dungeon;
    std::list<std::shared_ptr<GlCharacter>>  dungeon_objects;
    std::list<std::shared_ptr<IMapEvent>> map_events;
    std::list<std::shared_ptr<IMapEvent>> hero_events;
    std::list<std::shared_ptr<IMapEvent>> mob_events;
    std::list<std::string> m_messages;
    std::shared_ptr<GlCharacter> hero;
    std::string m_level_file;

    
    bool m_antialiase_enabled;
    std::string m_start_place;
    glLight Light,Light2;
    float light_angle;
    float light_radius;
    glCamera Camera;
    int now_frame;
    double time;
    float key_angle;
    glm::vec3 hero_position;
    glm::vec3 light_position;
    glm::vec3 light_dir_vector;
    glm::vec3 light_color_vector;

    LoaderUtility::LinesProcessor m_message_processor;
    
    void SaveObjects(const std::string &filename);

    void LoadMap(const std::string &filename,const std::string &start_place);
    void SelectStart(std::vector<std::string> &lines);
    void LoadTiles(std::vector<std::string> &lines);
    void LoadDungeonObjects(std::vector<std::string> &lines);
    bool AddObjectsFromFile(const std::string & object);
    bool AddObjectFromFile(const std::string & object,const std::string & name,glm::vec3 position);
    
    std::shared_ptr<GlCharacter> LoadObject(std::vector<std::string> &lines);
    void SetDungeonSize(std::vector<std::string> &lines);
    void LoadMapEvent(std::vector<std::string> &lines);

    void DrawGlobalCascade(const glRenderTargetDeffered &render_target);
    void PrerenderLight(glLight &Light,std::shared_ptr<GlCharacter>hero);
    void DrawGlobalLight(const GLuint light_loc, const glLight &Light);

    void DrawDungeon(GLuint current_shader, std::shared_ptr<GlCharacter>hero);
    void DrawLight(const glm::vec4 &light_pos_vector,const glm::vec3 &light_color_vector,glRenderTargetDeffered &render_target );
    void Draw2D(GLuint depth_map);
    void MoveHero(const glm::vec3 & hero_move);
    void FitObjects(int steps, float accuracy);
    std::pair<float,const glm::vec3>  FitObjectToMap(IGlModel& object, const glm::vec3 & position);
    float FitObjectToObject(IGlModel& object1,IGlModel& object2);
    InteractionResult ReactObjectToEvent(GlCharacter& object,IMapEvent& event,std::string &return_value);

    void DrawFxSprite(GLuint &current_shader, GLuint texture);
    void SetMapLight(std::vector<std::string> &lines);
    void MapObjectsEventsInteract();
    bool HeroEventsInteract(std::shared_ptr<GlCharacter> hero_ptr);
    bool MobKilled(std::shared_ptr<GlCharacter> obj);
    void PostMessage(const std::string & event_string);
    void ProcessMessages();
    
};

#endif
