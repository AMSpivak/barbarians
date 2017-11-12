#ifndef GL_I_GL_GAME_STATE
#define GL_I_GL_GAME_STATE
//#define GLEW_STATIC
//#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
//#include <vector>
#include <map>
#include "glscene.h"
#include "gl_texture_atlas.h"
class IGlGameState
{
private:

    //std::map<std::string,GLuint> &m_shader_map;
protected:
    std::map<std::string,GLuint> &m_shader_map;
    GLTextureAtlas &m_texture_atlas;
    
    size_t m_screen_width;
    size_t m_screen_height;
public:
    IGlGameState(std::map<std::string,GLuint> &shader_map,GLTextureAtlas &texture_atlas,

                    size_t screen_width, size_t screen_height):
                    m_shader_map(shader_map)
                    ,m_texture_atlas(texture_atlas)
    {
        
        m_screen_width = screen_width;
        m_screen_height = screen_height;
    }
    virtual ~IGlGameState(){}
    virtual void Draw() = 0;
    virtual IGlGameState * Process(std::map <int, bool> &inputs) = 0;
    virtual void SwitchIn() = 0;
    virtual void SwitchOut() = 0;
};


#endif
