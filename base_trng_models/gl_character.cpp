#include "gl_character.h"

GlCharacter::GlCharacter():
                            now_frame(0)
                            
{

}
GlCharacter::~GlCharacter()
{

}
void GlCharacter::Draw(GLuint shader)
{
    int models_count = Models.size();
    for(int i = 0; i < models_count; i++) Models[i]->Draw(shader,now_frame);
}
void GlCharacter::Process()
{
    now_frame++;
    if(now_frame == Animations[0]->frames.size()) now_frame = 3;
    
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
void GlCharacter::AddModel(std::string name)
{
    Models.emplace_back(std::make_shared<glModel>(name,Animations));
}