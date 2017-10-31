#include "gl_texture_atlas.h"
#include "glresourses.h"
#include <iostream>
GLTextureAtlas::GLTextureAtlas()
{}
GLTextureAtlas::~GLTextureAtlas()
{
    for(auto textyre_ptr : m_texture_map)
	{
        std::cout <<"Dlete texture: "<<textyre_ptr.first<<"\n";
		glDeleteTextures(1,textyre_ptr.second.get());
	}

}

std::shared_ptr<GLuint> GLTextureAtlas::AssignTexture(const std::string & FileName)
{
    std::map<std::string,std::shared_ptr<GLuint>>::iterator  it = m_texture_map.find(FileName);
    if( it != m_texture_map.end() )
        return it->second;
    
    //GLuint * texture = new;
    std::shared_ptr<GLuint> texture(new GLuint);
    m_texture_map.insert( std::pair<std::string,std::shared_ptr<GLuint>>(FileName,texture));
    LoadTexture(FileName,*(texture.get()));
    return texture;

}