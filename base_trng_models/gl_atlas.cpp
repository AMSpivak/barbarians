#include "gl_atlas.h"
#include "glresourses.h"
#include <iostream>
GLAtlas::GLAtlas(const std::string & ResourseFolder)
{
    m_resourse_folder = ResourseFolder;
}
GLAtlas::~GLAtlas()
{
    for(auto textyre_ptr : m_map)
	{
        std::cout <<"Delete texture: "<<textyre_ptr.first<<"\n";
        EmptyResource(textyre_ptr.second.get());
		//glDeleteTextures(1,textyre_ptr.second.get());
	}

}
void GLAtlas::FillResource(std::string filename,GLuint * resource)
{}

void GLAtlas::EmptyResource(GLuint * resource)
{}

std::shared_ptr<GLuint> GLAtlas::Assign(const std::string & FileName)
{
    std::map<std::string,std::shared_ptr<GLuint>>::iterator  it = m_map.find(FileName);
    if( it != m_map.end() )
    {
        std::cout<<"repeat\n";
        return it->second;
    }
    //GLuint * texture = new;
    std::shared_ptr<GLuint> resource(new GLuint);
    m_map.insert( std::pair<std::string,std::shared_ptr<GLuint>>(FileName,resource));
    FillResource(m_resourse_folder + FileName,resource.get());
    //LoadTexture(m_resourse_folder + FileName,*(resource.get()));
    std::cout<<"load:\n"<<FileName<<":"<<m_resourse_folder +FileName<<"\n";
    
    return resource;

}