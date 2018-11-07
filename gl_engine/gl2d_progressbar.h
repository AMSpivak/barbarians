#ifndef GL2D_PROGRESSBAR_H
#define GL2D_PROGRESSBAR_H
#include <memory>
#include "gl2d_item.h"
#include "i_gl_texture_struct.h"
namespace Gl2D
{
    class GlProgressbar : public Gl2dItem
    {
        private:
        std::shared_ptr<IGlTextureStruct> m_bar_texture;
        std::shared_ptr<IGlTextureStruct> m_fill_texture;
        //std::shared_ptr<IGlTextureStruct> m_map_texture_blurred;
        float m_progress;

        GLuint m_shader;
        public:
        GlProgressbar(float x,float y,float width, float height, float aspect_ratio,
                        std::shared_ptr<IGlTextureStruct> bar_texture,
                        std::shared_ptr<IGlTextureStruct> fill_texture,
                        GLuint shader):
                                                        Gl2dItem(x,y,width,height, aspect_ratio)
                                                        ,m_bar_texture(bar_texture)
                                                        ,m_fill_texture(fill_texture)
                                                        ,m_shader(shader)
                                                        ,m_progress(1.f)

                                                         {}
        ~GlProgressbar(){}
        void SetProgress(float value);
        void Draw();
    };

}

#endif