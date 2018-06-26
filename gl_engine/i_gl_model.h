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
    bool ghost;
    std::string m_name;
protected:
    glm::vec3 m_position;

public:
	int parent;
	int parent_attach_point;

    glm::mat4 model_matrix;
    float mass_inv;
    float radius;
    

    bool m_is_light;
    float m_light_radius;
    glm::vec3 m_light_color;
    glm::vec3 m_light_position;

    void SetPosition(const glm::vec3 &position)
    {
        m_position = position;
    }

    const glm::vec3 &GetPosition()
    {
        return m_position;
    }

    void SetName(const std::string &name)
    {
        m_name = name;
    }

    const std::string & GetName()
    {
        return m_name;
    }

    IGlModel():life_value(1.0),armor_value(1.0),ghost(false),model_matrix(),mass_inv(0),radius(1.0f), m_position(0.0f,0.0f,0.0f),m_is_light(false),m_name("unnamed")
    {

    }

    void SetLight(bool is_light,glm::vec3 light_color,glm::vec3 light_position,float light_radius)
    {
        m_is_light = is_light;
        m_light_radius = light_radius;
        m_light_color = light_color;
        m_light_position = light_position;
    }

    bool IsLight(glm::vec4 & ret_position, glm::vec3 &ret_color)
    {
        if(!m_is_light)
            return false;

        ret_position = glm::vec4(m_position[0],m_position[1],m_position[2],m_light_radius) 
                        + model_matrix * glm::vec4(m_light_position[0],m_light_position[1],m_light_position[2],0.0);
        ret_color = m_light_color;
        return true;
    }

    float GetLifeValue()
    {
        return life_value;
    }
    void SetLifeValue(float value)
    {
        life_value= value;
    }

     void SetArmorValue(float value)
    {
        armor_value= value;
    }

    void Damage(float damage)
    {
        life_value -= damage * armor_value;
    }

    virtual ~IGlModel(){}
    virtual void Draw(GLuint shader) const = 0;
    virtual void Draw(GLuint shader,const glm::mat4 &draw_matrix) = 0;
    virtual int Process() = 0;
    virtual void RefreshMatrixes() = 0;
    virtual int AddAxes(std::vector<glm::vec3> &axes) = 0;

    virtual std::pair<float, float> ProjectOnAxe(const glm::vec3 & axe) const = 0;
};


#endif

