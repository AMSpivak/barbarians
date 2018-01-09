#ifndef GL_I_MAP_EVENT
#define GL_I_MAP_EVENT
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "i_gl_model.h"
#include "glcamera.h"

#include <vector>

enum class InteractionResult { Nothing, Damage, Kill };
enum class EventProcessResult { Nothing, Kill };

class IMapEvent
{
private:

public:
    glm::mat4 model_matrix;
    float radius;
    float hero_dependence;
    float mobs_dependence;
    glm::vec3 position;
    IMapEvent():model_matrix(),radius(1.0f), position(0.0f,0.0f,0.0f)
    {

    }
    virtual ~IMapEvent(){}
    virtual InteractionResult Interact(IGlModel &model) =0;
    virtual EventProcessResult Process() = 0;
    virtual void Show(const glm::vec3 & offset, glCamera & camera){};
    virtual int AddAxes(std::vector<glm::vec3> &axes) = 0;
    virtual std::pair<float, float> ProjectOnAxe(const glm::vec3 & axe) = 0;
};

#endif