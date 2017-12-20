#include "collision.h"

std::pair<float, float> ProjectEdgesOnAxe( const glm::mat4 &model_matrix,std::vector<std::pair<glm::vec3, glm::vec3>> &edges,const glm::vec3 &position,const glm::vec3 & axe)
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    glm::vec3 norm;
    glm::vec4 norm4;
    float val;
    for(auto edge :edges)
    {

        norm4 = model_matrix * glm::vec4(edge.second[0],edge.second[1],edge.second[2],0.0f);
        norm = position + glm::vec3(norm4[0],norm4[1],norm4[2]) ;

        val = glm::dot(norm,axe);

        if(val < min) min = val;

        if(val > max) max = val;

        return std::pair<float, float>(min, max);

    }
    
}