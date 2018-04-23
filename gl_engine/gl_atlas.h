#ifndef GL_ATLAS
#define GL_ATLAS


//#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>
#include <iostream>

template <typename T>
class GLAtlas
{

public:
    GLAtlas(const std::string & ResourseFolder)
    {
        m_resourse_folder = ResourseFolder;
    }
    

    ~GLAtlas()
    {}

    std::shared_ptr<T> Assign(const std::string & filename)
    {
        auto it = m_map.find(filename);
        if( it != m_map.end() )
        {
            if (!it->second.expired())
            {
                std::cout << "locked "<< filename<<"\n";
                return it->second.lock();
            }
            else
            {
                std::cout << "reassign "<< filename<<"\n";
                
                auto ret = std::shared_ptr<T>(new T(m_resourse_folder + filename), Deleter(this, filename));
                it->second = ret;
                return ret;
            }

        }
        auto resource = std::shared_ptr<T>(new T(m_resourse_folder + filename), Deleter(this, filename));
        m_map.insert( std::pair<std::string,std::weak_ptr<T>>(filename,resource));
        
        return resource;

    }

    void Clean()
    {
    }

private:

    std::string m_resourse_folder;
    std::map<std::string,std::weak_ptr<T>> m_map;

    struct Deleter
    {
        Deleter(GLAtlas* atlas, const std::string& filename)
            : m_atlas(atlas)
            , m_filename(filename)
        {}

        void operator()(T* item)
        {
            m_atlas->m_map.erase(m_filename);
            delete item;
        }

        GLAtlas* m_atlas;
        std::string m_filename;
    };    



};


#endif