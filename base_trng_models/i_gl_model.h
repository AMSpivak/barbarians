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
	int parent_idx;
	int parent_bone;

    glm::mat4 model_matrix;

    
    virtual ~IGlModel() = 0;
    virtual void Draw() = 0;
};


#endif

