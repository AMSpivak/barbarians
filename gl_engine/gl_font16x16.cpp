#include "gl_font16x16.h"


GlText16x16::GlText16x16(const std::string & filename, GLAtlas<IGlTextureStruct> &m_texture_atlas,float width, float height)
    :IGlText( filename, m_texture_atlas,width, height)
{
	//LoadTexture(filename,m_texture);
	texture = m_texture_atlas.Assign(filename);
	
}

void GlText16x16::DrawString(const std::string &value,float x,float y, GLuint shader)
{
    renderSprite(shader, x, y, x + m_width, y, x + m_width, y + m_height, x, y + m_height,
	glm::vec4(1.0f,1.0f,1.0f,1.0f),&(texture->m_texture) );
}
