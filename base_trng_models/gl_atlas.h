#ifndef GL_ATLAS
#define GL_ATLAS


#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>
#include <iostream>

template <typename T>
class GLAtlas
{
private:
    std::string m_resourse_folder;
    std::map<std::string,std::shared_ptr<T>> m_map;
    virtual void FillResource(std::string filename,T * resource) {}
    virtual void EmptyResource(T * resource) {}
public:
    GLAtlas(const std::string & ResourseFolder)
    {
        m_resourse_folder = ResourseFolder;
    }
    

    ~GLAtlas()
    {
        for(auto resource_ptr : m_map)
        {
            std::cout <<"Delete resource: "<<resource_ptr.first<<"\n";
            EmptyResource(resource_ptr.second.get());
            //glDeleteTextures(1,textyre_ptr.second.get());
        }
    }

    std::shared_ptr<T> Assign(const std::string & FileName)
    {
        typename std::map<std::string,std::shared_ptr<T>>::iterator  it = m_map.find(FileName);
        if( it != m_map.end() )
        {
            std::cout<<"repeat\n";
            return it->second;
        }
        //GLuint * texture = new;
        std::shared_ptr<T> resource(new T);
        m_map.insert( std::pair<std::string,std::shared_ptr<T>>(FileName,resource));
        FillResource(m_resourse_folder + FileName,resource.get());
        //LoadTexture(m_resourse_folder + FileName,*(resource.get()));
        std::cout<<"load:\n"<<FileName<<":"<<m_resourse_folder +FileName<<"\n";
        
        return resource;

    }



};


#endif