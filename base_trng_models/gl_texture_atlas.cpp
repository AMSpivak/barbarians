#include "gl_texture_atlas.h"
#include "glresourses.h"
#include <iostream>
GLTextureAtlas::GLTextureAtlas(const std::string & ResourseFolder)
{
    m_resourse_folder = ResourseFolder;
}
GLTextureAtlas::~GLTextureAtlas()
{
    for(auto textyre_ptr : m_texture_map)
	{
        std::cout <<"Delete texture: "<<textyre_ptr.first<<"\n";
		glDeleteTextures(1,textyre_ptr.second.get());
	}

}

std::shared_ptr<GLuint> GLTextureAtlas::AssignTexture(const std::string & FileName)
{
    std::map<std::string,std::shared_ptr<GLuint>>::iterator  it = m_texture_map.find(FileName);
    if( it != m_texture_map.end() )
    {
        std::cout<<"repeat\n";
        return it->second;
    }
    //GLuint * texture = new;
    std::shared_ptr<GLuint> texture(new GLuint);
    m_texture_map.insert( std::pair<std::string,std::shared_ptr<GLuint>>(FileName,texture));
    LoadTexture(m_resourse_folder + FileName,*(texture.get()));
    std::cout<<"load\n"<<FileName<<":"<<m_resourse_folder +FileName<<"\n";;
    
    return texture;

}