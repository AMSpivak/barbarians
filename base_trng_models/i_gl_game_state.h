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
public:
    IGlGameState(std::map<std::string,GLuint> &shader_map)
    {
        m_shader_map = &shader_map;
    }
    virtual ~IGlGameState(){}
    virtual void Draw() = 0;
    virtual void Process() = 0;
};


#endif