#ifndef GL_TEXTURE_ATLAS
#define GL_TEXTURE_ATLAS


#include "gl_atlas.h"
#include "i_gl_texture_struct.h"


class GLTextureAtlas: public GLAtlas<IGlTextureStruct>
{


public:
    GLTextureAtlas(const std::string & ResourseFolder);
    
};


#endif