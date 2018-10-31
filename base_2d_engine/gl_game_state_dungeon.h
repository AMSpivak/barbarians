#ifndef GL_I_GL_GAME_STATE_DUNGEON
#define GL_I_GL_GAME_STATE_DUNGEON
//#define GLEW_STATIC
//#include <GL/glew.h>
#include "i_gl_game_state.h"
#include "gl_render_target.h"
#include "loader.h"
#include "gl_font16x16.h"
#include "gl_map_browser.h"

#include <list>

class GlGameStateDungeon: public IGlGameState
{
public:
    GlGameStateDungeon(std::map<const std::string,GLuint> &shader_map,
        std::map<std::string,std::shared_ptr<glRenderTarget>> &render_target_map,
        GLResourcesManager &resources_manager,
        size_t screen_width,
        size_t screen_height);

    ~GlGameStateDungeon();
    void Draw();
    IGlGameState * Process(std::map <int, bool> &inputs, float joy_x, float joy_y);
    void SwitchIn(){}
    void SwitchOut(){}
private:
    std::shared_ptr<IGlText> m_gl_text;

    std::map<std::string,std::shared_ptr<glRenderTarget>> &m_render_target_map;

    Gl2D::GlMapBrowser * m_map_browser;

    //std::shared_ptr<IGlTextureStruct> fx_texture, skybox, debug_texture;
    std::shared_ptr<IGlTextureStruct> chip_texture;

    std::list<std::string> m_messages;

    std::string m_level_file;

    
    bool m_antialiase_enabled;
    int now_frame;
    double time;
    float key_angle;

    LoaderUtility::LinesProcessor m_message_processor;
    void PostMessage(const std::string & event_string);
    void ProcessMessages();
    void Draw2D(GLuint depth_map);

};

#endif
