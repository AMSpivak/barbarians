#ifndef GL_BONES_ATLAS
#define GL_BONES_ATLAS

#include "i_gl_jub_struct.h"
#include "gl_atlas.h"


class GLBonesAtlas: public GLAtlas<IGlJubStruct>
{
public:
    GLBonesAtlas(const std::string & ResourseFolder):GLAtlas<IGlJubStruct>(ResourseFolder)
    {}

    
};


#endif