#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <vector>
#include <memory>
#include "glresourses.h"
#include <iostream>



class glCamera
{
	glm::mat4 view;
    glm::mat4 projection;
	glm::mat4 full_matrix;
public:	
	const glm::mat4 &CameraMatrix()
	{
		full_matrix =  projection * view;
		return full_matrix;
	}
	void SetCameraLocation(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
	{
		view = glm::lookAt(position,target,up);
	}
	
	void SetCameraLens(float FOV, float aspect, float near, float far)
	{
		projection = glm::perspective(glm::radians(FOV), aspect, near, far);
	}
	void SetCameraLens_Orto(float x1, float x2,float y1, float y2, float near, float far)
	{
		projection = glm::ortho(x1, x2, y1, y2, near, far);
	}
};


class glLight : public glCamera
{
	GLuint depthMapFBO;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;	
	public:
	GLuint depthMap;
	glLight()
	{
		glGenFramebuffers(1, &depthMapFBO);  
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// comparison mode of the shadow map
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	}
	~glLight()
	{
		glDeleteTextures(1,&depthMap);
		glDeleteBuffers(1, &depthMapFBO);
	}
	
	void SetLigtRender()
	{
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
};

class glModel
{
	
	GLuint VBO, VBO_BONES, VBO_BONES_IDX, VAO;
	int vertexcount;
	int bonescount;
	std::shared_ptr<Animation> animation;
	
public:	
	int parent_idx;
	int parent_bone;
	
	glm::mat4 model;
	GLuint diffuse_texture;	
	std::vector <Bone> bones;
	
	glModel()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VBO_BONES);
		glGenBuffers(1, &VBO_BONES_IDX);
	}
	
	glModel(std::string FileName)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VBO_BONES);
		glGenBuffers(1, &VBO_BONES_IDX);
		LoadAll(FileName);
	}
	
	glModel(std::string FileName,std::vector <std::shared_ptr<Animation> > &animations)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VBO_BONES);
		glGenBuffers(1, &VBO_BONES_IDX);
		LoadAll(FileName,animations);
	}
	

	
	const glm::mat4 &ModelMatrix()
	{
		return model;
	}
	
	
	~glModel()
	{
		glDeleteTextures(1,&diffuse_texture);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &VBO_BONES);
		glDeleteBuffers(1, &VBO_BONES_IDX);
	}
	

	
	
	void LoadModel(std::string FileName);
	void LoadModelBones(std::string FileName);
	void LoadAll(std::string FileName);
	void LoadAll(std::string FileName,std::vector <std::shared_ptr<Animation> > &animations);
	void Draw();
	void Draw(GLuint shaderProgram, Animation &animation, int now_frame);
	void Draw(GLuint shaderProgram, int now_frame);
	void AttachAnimation(std::vector <std::shared_ptr<Animation> > &animations, std::string Filename);
};

class glComplexModel
{
	std::vector <glModel> Models;
//public:
//	void LoadModel
};


