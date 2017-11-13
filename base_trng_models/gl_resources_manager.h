#ifndef GL_RESOURCES_MANAGER
#define GL_RESOURCES_MANAGER
#include "gl_texture_atlas.h"
#include <string>
class GLResourcesManager
{
public:
GLTextureAtlas m_texture_atlas;
GLResourcesManager(const std::string & textures_folder):m_texture_atlas(textures_folder)
{}
~GLResourcesManager(){}
};

#endif