#include "i_gl_model.h"
#include <string>
#include "gl_model.h"

class GlCharacter: public IGlModel
{
public:
    GlCharacter();
    ~GlCharacter();
    void Draw(GLuint shader) ;
    void Process();
    void AddModel(std::string name);

private:
    size_t now_frame;
    std::vector <std::shared_ptr<glModel> > Models;
	std::vector <std::shared_ptr<Animation> > Animations;
};