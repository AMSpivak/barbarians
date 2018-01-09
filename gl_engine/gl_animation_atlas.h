#ifndef GL_ANIMATION_ATLAS
#define GL_ANIMATION_ATLAS

#include "glresourses.h"
#include "gl_atlas.h"


class GLAnimationAtlas: public GLAtlas<Animation>
{

private:
    void FillResource(std::string filename,IGlJalStruct * resource)
    { }

    void EmptyResource(IGlJalStruct * resource){}
public:
    GLAnimationAtlas(const std::string & ResourseFolder):GLAtlas<Animation>(ResourseFolder)
    {}

    
};


#endif