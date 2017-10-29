#ifndef GL_RESOURSES
#define GL_RESOURSES

#include <GL/glew.h>
#include <string>
#include <SOIL/SOIL.h>
#include <vector>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



std::istream& operator>> ( std::istream& is, glm::mat4& mat) ;



std::string readShaderFile(std::string FileName);

GLuint LoadShader(std::string FileName,GLenum shaderType);
GLuint LinkShaderProgram(GLuint * shaders, int shaders_count);
GLuint LoadshaderProgram(std::string FileNameVS,std::string FileNameFS);

void EmptyShaders(GLuint * shaders, int shaders_count);


void LoadTexture(const std::string &FileName,GLuint &texture);

void renderQuad();
void Fit_Matrix(glm::mat4 &matrix,float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3);

void renderSprite(GLuint current_shader,
	float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3,
	const glm::vec4 & corrector_v,
	const GLuint * texture 
);

#endif
