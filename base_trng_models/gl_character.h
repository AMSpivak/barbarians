#ifndef GL_CHARACTER
#define GL_CHARACTER
#include "i_gl_model.h"
#include "animation_sequence.h"
#include <string>
#include <vector>
#include <map>

#include "gl_model.h"

class GlCharacter: public IGlModel
{
public:
    GlCharacter();
    ~GlCharacter();
    void Draw(GLuint shader) const;
    void Draw(GLuint shader,const glm::mat4 &draw_matrix);
    int Process();
    void AddModel(std::string name);
    void AddSequence(const std::string & name, const AnimationSequence & in_sequence);
    void UseSequence(const std::string & name);
    void Damage(float damage);
    void RefreshMatrixes();

    int AddAxes(std::vector<glm::vec3> &axes);
    std::pair<float, float> ProjectOnAxe(const glm::vec3 & axe) const;
    void AddEdge(const std::pair<glm::vec3, glm::vec3> edge);
    void UpdateFromLines(std::vector<std::string> &lines);
    void ToStream(std::ostream& os) const;

private:
    //glm::mat4 m_draw_matrix;
    size_t engine_frame;

    size_t now_frame;
    std::vector <std::shared_ptr<glModel> > Models;
    std::list<std::string> model_list;
    std::vector<std::pair<glm::vec3, glm::vec3>> m_edges; 
    std::map<std::string, AnimationSequence> sequence;
    AnimationSequence * current_animation;

    //void RefreshMatrixes();

};

void UpdateCharacterFromFile(const std::string &filename,GlCharacter & character);
std::ostream& operator << ( std::ostream& os, const GlCharacter & character);

#endif
