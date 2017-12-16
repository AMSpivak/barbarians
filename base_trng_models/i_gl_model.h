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
    float life_value;
    float armor_value;
public:
	int parent;
	int parent_attach_point;

    glm::mat4 model_matrix;
    float mass_inv;
    float radius;
    glm::vec3 position;
    IGlModel():life_value(1.0),model_matrix(),mass_inv(0),radius(1.0f), position(0.0f,0.0f,0.0f)
    {

    }
    float GetLifeValue()
    {
        return life_value;
    }
    void SetLifeValue(float value)
    {
        life_value= value;
    }
    void Damage(float damage)
    {
        life_value -= damage * armor_value;
    }

    virtual ~IGlModel(){}
    virtual void Draw(GLuint shader) = 0;
    virtual int Process() = 0;
    virtual void RefreshMatrixes() = 0;
    virtual int AddAxes(std::vector<glm::vec3> &axes) = 0;

    virtual std::pair<float, float> ProjectOnAxe(const glm::vec3 & axe) = 0;
};


#endif

