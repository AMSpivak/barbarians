#ifndef GL_I_GL_MODEL
#define GL_I_GL_MODEL
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <vector>
#include "glscene.h"

class IGlModel
{
private:

public:
	int parent;
	int parent_attach_point;

    glm::mat4 model_matrix;

    IGlModel():model_matrix()
    {}
    virtual ~IGlModel(){}
    virtual void Draw(GLuint shader) = 0;
    virtual void Process() = 0;
};


#endif

