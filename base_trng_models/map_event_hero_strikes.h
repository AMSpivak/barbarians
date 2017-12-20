#ifndef GL_ME_HERO_STRIKES
#define GL_ME_HERO_STRIKES
#include "i_map_event.h"


class IMapEventHeroStrike: public IMapEvent
{
private:
    std::vector<std::pair<glm::vec3, glm::vec3>> m_edges;    

public:
    glm::mat4 model_matrix;
    float radius;
    float damage;
    float mobs_dependence;
    glm::vec3 position;
    int frames_lifetime;
    IMapEventHeroStrike():model_matrix(),radius(1.0f), position(0.0f,0.0f,0.0f)
    {
    }

    ~IMapEventHeroStrike(){}
    int Process();
    int AddAxes(std::vector<glm::vec3> &axes);
    void AddEdge(const std::pair<glm::vec3, glm::vec3> edge);
    std::pair<float, float> ProjectOnAxe(const glm::vec3 & axe) = 0;
};

#endif