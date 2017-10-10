#ifndef GL_I_GL_GAME_STATE
#define GL_I_GL_GAME_STATE
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
//#include <vector>
#include <map>
#include "glscene.h"

class IGlGameState
{
private:
    std::map<std::string,GLuint> * m_shader_map;
    size_t m_screen_width;
    size_t m_screen_height;
public:
    IGlGameState(std::map<std::string,GLuint> &shader_map,size_t screen_width, size_t screen_height)
    {
        m_shader_map = &shader_map;
        m_screen_width = screen_width;
        m_screen_height = screen_height;
    }
    virtual ~IGlGameState(){}
    virtual void Draw() = 0;
    virtual void Process() = 0;
};


#endif
