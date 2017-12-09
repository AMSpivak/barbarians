#ifndef GL_ATLAS
#define GL_ATLAS


#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>
#include <iostream>

class GLAtlas
{
private:
    std::string m_resourse_folder;
    std::map<std::string,std::shared_ptr<GLuint>> m_map;
    virtual void FillResource(std::string filename,GLuint * resource);
    virtual void EmptyResource(GLuint * resource);
public:
    GLAtlas(const std::string & ResourseFolder);
    ~GLAtlas();
    std::shared_ptr<GLuint> Assign(const std::string & FileName);



};


#endif