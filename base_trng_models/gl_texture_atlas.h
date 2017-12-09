#ifndef GL_TEXTURE_ATLAS
#define GL_TEXTURE_ATLAS


#include "gl_atlas.h"


class GLTextureAtlas: public GLAtlas
{

private:
    void FillResource(std::string filename,GLuint * resource);
    void EmptyResource(GLuint * resource);
public:
    GLTextureAtlas(const std::string & ResourseFolder);
    
};


#endif