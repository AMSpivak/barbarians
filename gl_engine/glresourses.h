#ifndef GL_ReSOURSES
#define GL_ReSOURSES

#include <GL/glew.h>
#include <string>
#include <SOIL/SOIL.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Bone {
	std::string name;
	std::string parent;
	glm::mat4 matrix;
};

struct AnimationFrame
{
	std::vector <glm::mat4> bones;
	GLfloat * UnsafeReturnDataPtr()
	{
		return reinterpret_cast<GLfloat*>(glm::value_ptr(bones[0]));
	}
};

struct Animation
{
	int framescount;
	std::vector <AnimationFrame> frames;
	void LoadAnimation(std::string FileName, std::vector <Bone> &bones);
};


std::istream& operator>> ( std::istream& is, glm::mat4& mat) ;

std::istream& operator>> ( std::istream& is, Bone& bone) ;


std::string readShaderFile(std::string FileName);

GLuint LoadShader(std::string FileName,GLenum shaderType);
GLuint LinkShaderProgram(GLuint * shaders, int shaders_count);
GLuint LoadshaderProgram(std::string FileNameVS,std::string FileNameFS);

void EmptyShaders(GLuint * shaders, int shaders_count);

void LoadVertexArray(std::string FileName,GLuint &VBO, GLuint &VBO_BONES, GLuint &VBO_BONES_IDX, int &vertex_count);
void LoadBonesArray(std::string FileName,std::vector <Bone> &bones,int &bones_count);
void LoadTexture(std::string FileName,GLuint &texture);
void LoadCubemap(const std::string FileName,GLuint &texture);

void renderQuad();

void Fit_Matrix(glm::mat4 &matrix,float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3);

void renderSprite(GLuint current_shader,
	float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3,
	const glm::vec4 & corrector_v,
	const GLuint * texture 
);

void renderSpriteDepth(GLuint current_shader, GLuint depthmap, float sprite_depth,
	float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3,
	const glm::vec4 & corrector_v,
	const GLuint * texture 
);

#endif
