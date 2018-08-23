#include "gl_character.h"
#include <sstream>
#include "loader.h"
#include "engine_settings.h"

std::ostream& operator << ( std::ostream& os, const GlCharacter & character)
{
    os<<"<object>\n";
    character.ToStream(os);
    os<<"<!object>\n";
	return os;
}

GlCharacter::GlCharacter():
                            engine_frame(0)
                            ,now_frame(0)
                            ,current_animation(nullptr)
{

}

GlCharacter::~GlCharacter()
{

}

void GlCharacter::ToStream(std::ostream& os) const
{
    
    for(auto model_name : model_list)
    {
        os<<"model "<<model_name<<"\n";
    }

    for(auto seq : sequence)
    {
        os<<"sequence "<<seq.first<<" "<<seq.second.start_frame<<" "<<seq.second.end_frame<<"\n";
    }
    glm::mat4 tmp_matrix = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    os<<"matrix "<<tmp_matrix<<"\n"
    <<"mass_inv "<<mass_inv<<"\n"
    <<"armor "<<GetArmorValue()<<"\n"
    <<"life " << GetLifeValue()<<"\n"
    <<"name " << GetName()<<"\n"
    <<"radius "<<radius<<"\n"
    <<"position "<<GetPosition()<<"\n";

    if(m_is_light)
    {
        os<< "light "<<" "<< m_light_color <<" "<< m_light_position <<" "<< m_light_radius<<"\n";
    }

    if(!m_edges.empty())
    {

        //os<< "light "<<" "<< m_light_color <<" "<< m_light_position <<" "<< m_light_radius<<"\n";
    }
}


void UpdateCharacterFromFile(const std::string &filename,GlCharacter & character)
{
    std::ifstream char_file;
	char_file.open(filename);
    std::vector<std::string> lines;
    LoaderUtility::LoadLineBlock(char_file,"never_exists",lines);
    char_file.close(); 
    character.UpdateFromLines(lines); 
}



void GlCharacter::UpdateFromLines(std::vector<std::string> &lines)
{
    if(lines.size()<=1) 
    return;
    

    std::map<std::string,const std::function<void(std::stringstream&)>> execute_funcs;
    execute_funcs.insert(std::make_pair("model",[this](std::stringstream &sstream)
                                        {
                                            std::string name;
                                            sstream >> name;
                                            AddModel(name);
                                        }));

    execute_funcs.insert(std::make_pair("sequence",[this](std::stringstream &sstream)
                                        {
                                            size_t start =0;
                                            size_t end =0;
                                            std::string name;
                                            sstream >> name >> start >> end; 
                                            AnimationSequence sequence(start,end);
                                            AddSequence(name,sequence);
                                            UseSequence(name);
                                        }));
    execute_funcs.insert(std::make_pair("run_sequence",[this](std::stringstream &sstream)
                                        {
                                            size_t start =0;
                                            size_t end =0;
                                            std::string name;
                                            sstream >> name;
                                            UseSequence(name);
                                        }));
    execute_funcs.insert(std::make_pair("orientation",[this](std::stringstream &sstream)
                                        {
                                            float a_x = 0.0f;
                                            float a_y = 0.0f;
                                            float a_z = 0.0f;

                                            sstream >> a_x >> a_y >> a_z; 
                                            
                                            model_matrix = glm::rotate(model_matrix, glm::radians(a_x), glm::vec3(1.0f, 0.0f, 0.0f));
                                            model_matrix = glm::rotate(model_matrix, glm::radians(a_y), glm::vec3(0.0f, 1.0f, 0.0f));
                                            model_matrix = glm::rotate(model_matrix, glm::radians(a_z), glm::vec3(0.0f, 0.0f, 1.0f));
                                        }));
    execute_funcs.insert(std::make_pair("matrix",[this](std::stringstream &sstream)
                                        {
                                            sstream >> model_matrix; 
                                         }));
    execute_funcs.insert(std::make_pair("mass_inv",[this](std::stringstream &sstream)
                                        {
                                            sstream >> mass_inv;
                                        }));

    execute_funcs.insert(std::make_pair("armor",[this](std::stringstream &sstream)
                                        {
                                            float armor = 1.0f;
                                            sstream >> armor;
                                            SetArmorValue(armor);
                                        }));

    execute_funcs.insert(std::make_pair("life",[this](std::stringstream &sstream)
                                        {
                                            float life = 1.0f;
                                            sstream >> life;
                                            SetLifeValue(life);
                                        }));
    execute_funcs.insert(std::make_pair("name",[this](std::stringstream &sstream)
                                        {   std::string name;
                                            sstream >> name;
                                            SetName(name);
                                        }));

    execute_funcs.insert(std::make_pair("light",[this](std::stringstream &sstream)
                                        {
                                            
                                            float light_radius = 0.0f;
                                            glm::vec3 color;
                                            glm::vec3 l_position;
                                            sstream >> color >> l_position >> light_radius; 
                                            SetLight(true,color,l_position,light_radius);
                                        }));

    execute_funcs.insert(std::make_pair("radius",[this](std::stringstream &sstream)
                                        {
                                            sstream >> radius;
                                        }));
                                        
    execute_funcs.insert(std::make_pair("edge",[this](std::stringstream &sstream)
                                        {
                                            glm::vec3 start;
                                            glm::vec3 stop;
                                            sstream >> start>>stop;
                                            AddEdge(std::make_pair(start,stop));
                                        }));

    execute_funcs.insert(std::make_pair("position",[this](std::stringstream &sstream)
                                        {
                                            float a_x = 0.0f;
                                            float a_y = 0.0f;
                                            float a_z = 0.0f;

                                            sstream >> a_x >> a_y >> a_z; 
                                            SetPosition(glm::vec3(a_x,a_y,a_z));                                    
                                        }));  

    


    for(auto s : lines)
    {
        std::stringstream ss(s);
        std::string parameter;
        ss >> parameter;
        try
        {
            execute_funcs.at(parameter)(ss);
            //(execute_funcs[info])(ss);
            
        }
        catch(const std::out_of_range& exp)
        {
            std::cout<<"Unknown model parameter: "<<s<<"\n";
        } 
    }
    //model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

void GlCharacter::AddEdge(const std::pair<glm::vec3, glm::vec3> edge)
{
    m_edges.push_back(edge);
}

void GlCharacter::UseSequence(const std::string & name)
{
    current_animation = &sequence[name];
}

void GlCharacter::AddSequence(const std::string & name, const AnimationSequence & in_sequence)
{
    sequence.insert( std::pair<std::string, AnimationSequence>(name,in_sequence));
}

void GlCharacter::Draw(GLuint shader) const
{
    for(auto model : Models) model->Draw(shader,now_frame);
}


void GlCharacter::Draw(GLuint shader,const glm::mat4 &draw_matrix)
{
    unsigned short frame = EngineSettings::GetEngineSettings()->GetFrame();
    if(engine_frame != frame)
    {
      
        for(auto model : Models) model->SetDrawMatrix(draw_matrix);
        //RefreshMatrixes();
        Draw(shader);
        engine_frame = frame;
    }
    else
    {
        Draw(shader);
        //std::cout<<"fast_draw\n";
    }
    
}



void GlCharacter::RefreshMatrixes()
{
    int models_count = Models.size();
    for(int i = 0; i < models_count; i++)
    if(Models[i]->parent_idx != -1)
    {
        IGlJubStruct * bone_ptr = Models[Models[i]->parent_idx]->jub_bones.get();
        Models[i]-> model = Models[Models[i]->parent_idx]->model *
            Models[Models[i]->parent_idx]->GetBoneMatrix(now_frame,Models[i]->parent_bone) *
           // Models[Models[i]->parent_idx]->animation->frames[now_frame].bones[Models[i]->parent_bone] *
           bone_ptr->bones[Models[i]->parent_bone].matrix * glm::inverse(Models[i]-> jub_bones.get()->bones[0].matrix);
    }
    else
    {
        Models[i]-> model = model_matrix;
    }
}

int GlCharacter::Process()
{
    if(GetLifeValue() <=0.0f) return 1;
    if(current_animation == nullptr)
    {
        now_frame = 0;
    }
    else
    {
        ++now_frame;
        ++now_frame;


        if(now_frame > current_animation->end_frame || now_frame < current_animation->start_frame) now_frame = current_animation->start_frame;
    }

    RefreshMatrixes();
    return 0;
}

void GlCharacter::Damage(float damage)
{
    UseSequence("damage");
    IGlModel::Damage(damage);
}

void GlCharacter::AddModel(std::string name)
{
    model_list.emplace_back(name);
    Models.emplace_back(std::make_shared<glModel>(name));
}

int GlCharacter::AddAxes(std::vector<glm::vec3> &axes)
{
    if(m_edges.emty()) 
        return 0;
    return Collision::AddAxes(axes,m_edges,model_matrix);

    
}

std::pair<float, float> GlCharacter::ProjectOnAxe(const glm::vec3 &axe) const
{
    if(m_edges.emty()) 
    {
        float position_on_axe = glm::dot(axe, m_position);
        std::pair<float, float> ret_value(position_on_axe - radius, position_on_axe + radius);
        return ret_value;
    }
    else
        return Collision::ProjectEdgesOnAxe(model_matrix,m_edges,position,axe);
}
