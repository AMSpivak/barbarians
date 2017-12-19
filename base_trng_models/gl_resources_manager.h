#ifndef GL_RESOURCES_MANAGER
#define GL_RESOURCES_MANAGER
#include "gl_texture_atlas.h"
#include "gl_jal_mesh_atlas.h"
#include "gl_bones_atlas.h"
#include "gl_animation_atlas.h"
#include <string>
class GLResourcesManager
{
public:
GLTextureAtlas m_texture_atlas;
GLJalMeshAtlas m_mesh_atlas;
GLAnimationAtlas m_animation_atlas;
GLBonesAtlas m_bones_atlas;
GLResourcesManager(const std::string & textures_folder,
                    const std::string & meshes_folder,
                    const std::string & animation_folder,
                    const std::string & bones_folder
                    )
                    :m_texture_atlas(textures_folder)
                    ,m_mesh_atlas(meshes_folder)
                    ,m_animation_atlas(animation_folder)
                    ,m_bones_atlas(bones_folder)
{}
~GLResourcesManager(){}
};

GLResourcesManager * GetResourceManager();
void SetResourceManager(GLResourcesManager * resource_manager);

#endif