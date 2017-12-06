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
#include "gl_resources_manager.h"
class IGlGameState
{
private:

    //std::map<std::string,GLuint> &m_shader_map;
protected:
    std::map<std::string,GLuint> &m_shader_map;
    GLResourcesManager &m_resources_manager;
    
    size_t m_screen_width;
    size_t m_screen_height;
public:
    IGlGameState(std::map<std::string,GLuint> &shader_map,GLResourcesManager &resources_manager,

                    size_t screen_width, size_t screen_height):
                    m_shader_map(shader_map)
                    ,m_resources_manager(resources_manager)
    {
        
        m_screen_width = screen_width;
        m_screen_height = screen_height;
    }
    virtual ~IGlGameState(){}
    virtual void Draw() = 0;
    virtual IGlGameState * Process(std::map <int, bool> &inputs, float joy_x, float joy_y) = 0;
    virtual void SwitchIn() = 0;
    virtual void SwitchOut() = 0;
};


#endif
