#include "gl_character.h"

GlCharacter::GlCharacter():
                         now_frame(0)
                        ,current_animation(nullptr)
{

}

GlCharacter::~GlCharacter()
{

}

void GlCharacter::UseSequence(const std::string & name)
{
    current_animation = &sequence[name];
}

void GlCharacter::AddSequence(const std::string & name, const AnimationSequence & in_sequence)
{
    sequence.insert( std::pair<std::string, AnimationSequence>(name,in_sequence));
}

void GlCharacter::Draw(GLuint shader)
{

    int models_count = Models.size();
    for(int i = 0; i < models_count; i++) Models[i]->Draw(shader,now_frame);
}
void GlCharacter::RefreshMatrixes()
{
    int models_count = Models.size();
    for(int i = 0; i < models_count; i++)
    if(Models[i]->parent_idx != -1)
    {
        Models[i]-> model = Models[Models[i]->parent_idx]->model *
         Animations[Models[i]->parent_idx]->frames[now_frame].bones[Models[i]->parent_bone] *
          Models[Models[i]->parent_idx]->bones[Models[i]->parent_bone].matrix *
          glm::inverse(Models[i]-> bones[0].matrix)
          ;
    }
    else
    {
        Models[i]-> model = model_matrix;
    }
}

void GlCharacter::Process()
{
    if(current_animation == nullptr)
    {
        now_frame = 0;
    }
    else
    {
        now_frame++;

        if(now_frame > current_animation->end_frame || now_frame < current_animation->start_frame) now_frame = current_animation->start_frame;
    }

    RefreshMatrixes();
}
void GlCharacter::AddModel(std::string name)
{
    Models.emplace_back(std::make_shared<glModel>(name,Animations));
}

int GlCharacter::AddAxes(std::vector<glm::vec3> &axes)
{
    return 0;
}

std::pair<float, float> GlCharacter::ProjectOnAxe(glm::vec3 axe)
{
    float position_on_axe = glm::dot(axe, position);

    std::pair<float, float> ret_value(position_on_axe - radius, position_on_axe + radius);

}
