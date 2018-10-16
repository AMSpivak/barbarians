#ifndef GL_TEXT
#define GL_TEXT
#include <GL/glew.h>
#include <string>
#include "glresourses.h"
class IGlText
{
    protected:
    float m_width;
    float m_height;
    public:
    IGlText(const std::string & filename, GLAtlas<IGlTextureStruct> &m_texture_atlas,float width, float height)
    {
        //LoadTexture(filename,m_texture);
        SetTextSize(width,height);
    }
    
    
    ~IGlText()
    {
    }

    void SetTextSize(float width, float height)
    {
        m_width = width;
        m_height = height;
    }
    virtual void DrawString(const std::string &value,float x,float y, GLuint shader) = 0;
};
#endif