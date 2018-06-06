#ifndef GL_ENGINE_PHYSICS
#define GL_ENGINE_PHYSICS

#include "collision.h"
namespace Physics
{
    template <typename   T1, typename T2>
    std::pair<float,glm::vec3> Intersection(T1 &object1,T2 &object2)
    {
        std::vector < glm::vec3 > axes;
        axes.push_back(glm::normalize(object2.position - object1.position));
        object1.AddAxes(axes);
        object2.AddAxes(axes);

        glm::vec3 compensate_axe(0.0f,0.0f,0.0f);
        float intersection = std::numeric_limits<float>::max();

        for(auto axe : axes)
        {
            std::pair<float,float> projection1 = object1.ProjectOnAxe(axe);
            std::pair<float,float> projection2 = object2.ProjectOnAxe(axe);
            float axe_intersection = CollisionOnAxe(projection1,projection2);

            if(axe_intersection < std::numeric_limits<float>::min())
                return std::make_pair((float)0.0f,compensate_axe);

            if(axe_intersection < intersection)
            {
                compensate_axe = axe;
                intersection = axe_intersection;
            }
        }
        return std::make_pair(intersection,compensate_axe);
    }
}

#endif