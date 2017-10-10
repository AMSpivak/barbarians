#include "gl_game_state_arena.h"
#include "glm/glm.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glscene.h"

IGlGameStateArena::IGlGameStateArena(std::map<std::string,GLuint> &shader_map,
                                    std::map<std::string,std::shared_ptr<glRenderTarget>> * render_target_map,
                                    std::map<std::string,std::shared_ptr<IGlModel>> * models_map,
                                    size_t screen_width,
                                    size_t screen_height):
                                                        IGlGameState(shader_map,screen_width,screen_height),
                                                        m_render_target_map(render_target_map)
{
    Models.emplace_back(std::make_shared<glModel>("material/scene03/scene.mdl",Animations));
    glCamera Camera;
    //Camera.SetCameraLocation(glm::vec3(12.0f, 2.0f, 0.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Camera.SetCameraLocation(glm::vec3(12.0f, 8.485f, -12.0f),glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Camera.SetCameraLens(45,(float)screen_width / (float)screen_height,0.1f, 100.0f);

    glLight Light;

    float light_angle = 90.0f;
    float light_radius = 20.0f;

    glm::vec3 light_position = glm::vec3(0.0f, light_radius*glm::sin(glm::radians(light_angle)), -light_radius*glm::cos(glm::radians(light_angle)));

    Light.SetCameraLocation(light_position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    float f_near = 1.f;
    float f_far = 35.0f;
    Light.SetCameraLens_Orto(-20.0f, 20.0f,-20.0f, 20.0f,f_near,f_far);
}

void IGlGameStateArena::Draw()
{
    glRenderTargetDeffered &render_target = *(dynamic_cast<glRenderTargetDeffered*>((*m_render_target_map)["base_deffered"].get()));
    glRenderTarget &final_render_target = *((*m_render_target_map)["final"].get());


}
void IGlGameStateArena::Process()
{



}
