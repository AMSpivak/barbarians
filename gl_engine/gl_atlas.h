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
    std::map<std::string,std::weak_ptr<T>> m_map;
public:
    GLAtlas(const std::string & ResourseFolder)
    {
        m_resourse_folder = ResourseFolder;
    }
    

    ~GLAtlas()
    {}

    std::shared_ptr<T> Assign(const std::string & FileName)
    {
        typename std::map<std::string,std::weak_ptr<T>>::iterator  it = m_map.find(FileName);
        if( it != m_map.end() )
        {
            if (!it->second.expired())
            {
                std::cout << "locked "<< FileName<<"\n";
                return it->second.lock();
            }
            else
            {
                std::cout << "reassign "<< FileName<<"\n";
                
                auto ret = std::make_shared<T>(m_resourse_folder + FileName);
                it->second = ret;
                return ret;
            }

        }
        std::cout << "insert "<< FileName<<"\n";
        auto resource = std::make_shared<T>(m_resourse_folder + FileName);
        m_map.insert( std::pair<std::string,std::weak_ptr<T>>(FileName,resource));
        
        return resource;

    }

    void Clean()
    {
        /*
        typename std::map<std::string,std::shared_ptr<T>>::iterator cur = m_map.begin();
        typename std::map<std::string,std::shared_ptr<T>>::iterator end = m_map.end();

        while (cur != end)
        {
            if ((*cur).second.unique())
            {
                std::cout <<"Delete resource: "<<(*cur).first<<"\n";
                
                //EmptyResource((*cur).second.get());
                m_map.erase(cur++); 
            }  
            else
            {
                ++cur;
            }
        }*/
    }



};


#endif