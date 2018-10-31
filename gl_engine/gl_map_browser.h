#ifndef GL_MAP_BROWSER
#define GL_MAP_BROWSER
#include <memory>
#include "gl2d_item.h"
#include "i_gl_texture_struct.h"
namespace Gl2D
{
    class GlMapBrowser : public Gl2dItem
    {
        private:
        std::shared_ptr<IGlTextureStruct> m_map_texture;
        //std::shared_ptr<IGlTextureStruct> m_map_texture_blurred;
        float m_zoom;
        float m_map_x;
        float m_map_y;
        GLuint m_shader;
        public:
        GlMapBrowser(float x,float y,float width, float height, float aspect_ratio, std::shared_ptr<IGlTextureStruct> map_texture,GLuint shader):
                                                        Gl2dItem(x,y,width,height, aspect_ratio), m_map_texture(map_texture)
                                                        ,m_shader(shader)
                                                        ,m_zoom(1.f)
                                                        ,m_map_x(0.f)
                                                        ,m_map_y(0.f)

                                                         {}
        ~GlMapBrowser(){}
        void SetZoom(float value);
        float GetZoom();
        void MoveMap(float x, float y);
        void Draw();
    };

}

#endif