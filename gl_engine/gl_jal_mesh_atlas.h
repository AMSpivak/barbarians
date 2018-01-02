#ifndef GL_MESH_ATLAS
#define GL_MESH_ATLAS

#include "i_gl_jal_struct.h"
#include "gl_atlas.h"


class GLJalMeshAtlas: public GLAtlas<IGlJalStruct>
{

private:
    void FillResource(std::string filename,IGlJalStruct * resource);
    void EmptyResource(IGlJalStruct * resource);
public:
    GLJalMeshAtlas(const std::string & ResourseFolder);
    
};


#endif