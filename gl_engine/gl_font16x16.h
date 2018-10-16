#ifndef GL_TEXT16x16
#define GL_TEXT16x16
#include "i_gl_font.h"
class GlText16x16 : public IGlText
{
    float m_width;
    float m_height;
    public:
    GlText16x16(const std::string & filename, GLAtlas<IGlTextureStruct> &m_texture_atlas,float width, float height)
    {
        //LoadTexture(filename,m_texture);
        SetTextSize(width,height);
    }
    
    
    ~GlText16x16()
    {
    }

    virtual void DrawString(const std::string &value,float x,float y, GLuint shader);

};
#endif