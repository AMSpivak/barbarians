#include "gl_jal_mesh_atlas.h"
#include "glresourses.h"
#include <iostream>

GLJalMeshAtlas::GLJalMeshAtlas(const std::string & ResourseFolder):GLAtlas<IGlJalStruct>(ResourseFolder)
{}


void GLJalMeshAtlas::FillResource(std::string filename,IGlJalStruct * resource)
{
    //LoadTexture(filename,*(resource));
}


void GLJalMeshAtlas::EmptyResource(IGlJalStruct * resource)
{
    //glDeleteTextures(1,resource);
}