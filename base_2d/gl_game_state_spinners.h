#ifndef GL_I_GL_GAME_STATE_SPINNER
#define GL_I_GL_GAME_STATE_SPINNER

#include "i_gl_game_state.h"
#include "gl_spinner.h"

class GlGameStateSpinner: public IGlGameState
{
public:
    GlGameStateSpinner(std::map<std::string,GLuint> &shader_map,
        GLTextureAtlas &texture_atlas,
        size_t screen_width,
        size_t screen_height);

    ~GlGameStateSpinner(){}
    void Draw();
    IGlGameState * Process(std::map <int, bool> &inputs);
private:
    GlSpinner spinner;
    double time;
    size_t spinning_count;
    float speeds[5] ={0};
    float angles[5] ={0};
    //std::vector <std::shared_ptr<GLuint> > textures;

};

#endif