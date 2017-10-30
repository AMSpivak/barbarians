#ifndef GL_TEXTURE_ATLAS
#define GL_TEXTURE_ATLAS


#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>

class GLTextureAtlas
{
private:
    std::map<std::string,std::shared_ptr<GLuint>> m_texture_map;
public:
    GLTextureAtlas();
    ~GLTextureAtlas();
    std::shared_ptr<GLuint> AssignTexture(const std::string & FileName);

};


#endif