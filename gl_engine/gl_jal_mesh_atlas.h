#ifndef GL_MESH_ATLAS
#define GL_MESH_ATLAS

#include "i_gl_jal_struct.h"
#include "gl_atlas.h"


class GLJalMeshAtlas: public GLAtlas<IGlJalStruct>
{


public:
    GLJalMeshAtlas(const std::string & ResourseFolder);
    
};


#endif