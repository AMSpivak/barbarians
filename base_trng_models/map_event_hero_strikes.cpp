#include "map_event_hero_strikes.h"
#include "collision.h"

void IMapEventHeroStrike::AddEdge(const std::pair<glm::vec3, glm::vec3> edge)
{
    m_edges.push_back(edge);
}

void IMapEventHeroStrike::Show(const glm::vec3 & offset, glCamera & camera)
{
    /*renderBillBoardDepth(m_current_shader,m_depthmap,m_texture,   
        m_width,m_height,position,offset,camera);*/
/*
    glm::vec4 norm4;
    glm::vec3 norm;

    for(auto edge :m_edges)
    {
        norm4 = model_matrix * glm::vec4(edge.second[0],edge.second[1],edge.second[2],1.0f);
        norm = position + glm::vec3(norm4[0],norm4[1],norm4[2]);
        renderBillBoardDepth(m_current_shader,m_depthmap,m_texture,   
            m_width,m_height,glm::vec4(1.0,1.0,1.0,1.0),norm,offset,camera);
    }*/

}


int IMapEventHeroStrike::AddAxes(std::vector<glm::vec3> &axes)
{
    glm::vec4 norm4;
    glm::vec3 norm;
    glm::vec3 norm_z(0.0f,0.0f,1.0f);

    for(auto edge :m_edges)
    {
        norm4 = model_matrix * glm::vec4(edge.second[0],edge.second[1],edge.second[2],0.0f);
        norm = glm::vec3(norm4[0],norm4[1],norm4[2]);        
        axes.push_back(glm::normalize(norm));
        norm = edge.second - edge.first;
        /*float tmp = norm[0];
        norm[0] = norm[2];
        norm[2] = -tmp;*/
        norm = glm::cross(norm_z,norm);
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

EventProcessResult IMapEventHeroStrike::Process()
{
    //std::cout<<"Stop event\n";
    return EventProcessResult::Kill;
}

InteractionResult IMapEventHeroStrike::Interact(IGlModel &model)
{
    model.Damage(0.1f);
    //std::cout<<"life "<<model.GetLifeValue()<<"\n";
    if(model.GetLifeValue()< 0)
        return InteractionResult::Kill;
    return InteractionResult::Damage;
}