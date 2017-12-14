#include "gl_texture_atlas.h"
#include "glresourses.h"
#include <iostream>

GLTextureAtlas::GLTextureAtlas(const std::string & ResourseFolder):GLAtlas<GLuint>(ResourseFolder)
{}


void GLTextureAtlas::FillResource(std::string filename,GLuint * resource)
{
    LoadTexture(filename,*(resource));
}


void GLTextureAtlas::EmptyResource(GLuint * resource)
{
    glDeleteTextures(1,resource);
}