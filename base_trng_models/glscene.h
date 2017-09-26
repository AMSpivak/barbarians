#ifndef GL_ENGINE_SCENE
#define GL_ENGINE_SCENE

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

#endif
