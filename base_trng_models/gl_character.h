#ifndef GL_CHARACTER
#define GL_CHARACTER
#include "i_gl_model.h"
#include "animation_sequence.h"
#include <string>
#include <map>

#include "gl_model.h"

class GlCharacter: public IGlModel
{
public:
    GlCharacter();
    ~GlCharacter();
    void Draw(GLuint shader) ;
    int Process();
    void AddModel(std::string name);
    void AddSequence(const std::string & name, const AnimationSequence & in_sequence);
    void UseSequence(const std::string & name);
    void Damage(float damage);
    void RefreshMatrixes();

    int AddAxes(std::vector<glm::vec3> &axes);
    std::pair<float, float> ProjectOnAxe(const glm::vec3 & axe);

private:

    size_t now_frame;
    std::vector <std::shared_ptr<glModel> > Models;
	std::vector <std::shared_ptr<Animation> > Animations;
    //std::map<std::string, GLuint> &shader_map;
    std::map<std::string, AnimationSequence> sequence;
    AnimationSequence * current_animation;

    //void RefreshMatrixes();

};
#endif
