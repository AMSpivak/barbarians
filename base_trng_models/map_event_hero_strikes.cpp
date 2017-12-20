#include "map_event_hero_strikes.h"
#include "collision.h"

void IMapEventHeroStrike::AddEdge(const std::pair<glm::vec3, glm::vec3> edge)
{
    m_edges.push_back(edge);
}

int IMapEventHeroStrike::AddAxes(std::vector<glm::vec3> &axes)
{
    glm::vec4 norm4;
    glm::vec3 norm;

    for(auto edge :m_edges)
    {
        norm4 = model_matrix * glm::vec4(edge.second[0],edge.second[1],edge.second[2],0.0f);
        norm = glm::vec3(norm4[0],norm4[1],norm4[2]);        
        axes.push_back(glm::normalize(norm));
        norm = edge.second - edge.first;
        float tmp = norm[0];
        norm[0] = norm[2];
        norm[2] = -tmp;
        norm4 = model_matrix * glm::vec4(norm[0],norm[1],norm[2],0.0f);
        norm = glm::vec3(norm4[0],norm4[1],norm4[2]);
        axes.push_back(glm::normalize(norm));
    }
    
    return 2 * m_edges.size();
}

std::pair<float, float> IMapEventHeroStrike::ProjectOnAxe(const glm::vec3 & axe)
{
    return ProjectEdgesOnAxe(model_matrix,m_edges,position,axe);
}
