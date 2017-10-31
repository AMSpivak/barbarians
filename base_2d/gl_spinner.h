#ifndef GL_SPINNER
#define GL_SPINNER

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include <string>
//#include <vector>
#include <map>
#include "glscene.h"
#include "gl_texture_atlas.h"
const double PI = 3.14159265;
class GlSpinner
{
public: 
    GlSpinner(GLTextureAtlas &texture_atlas,size_t sectors);

    ~GlSpinner(){}
    void Draw(GLuint spriteShader,float angle,float x_start, float x_end);
private:
    GLTextureAtlas &m_texture_atlas;
    size_t m_sectors;
    std::vector <std::shared_ptr<GLuint> > textures;

};

#endif