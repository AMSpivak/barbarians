#ifndef GL_MODEL
#define GL_MODEL
#include <GL/glew.h>

#include <string>
#include <vector>
#include <memory>
#include "glresourses.h"
#include <iostream>
#include "i_gl_jal_struct.h"

#include "gl_resources_manager.h"

class glModel
{

	IGlJalStruct jal_mesh;
	//GLuint VBO, VBO_BONES, VBO_BONES_IDX, VAO;
	int vertexcount;
	int bonescount;
	std::shared_ptr<Animation> animation;
	//GLResourcesManager &m_resources_manager;

public:
	int parent_idx;
	int parent_bone;

	glm::mat4 model;
	std::shared_ptr<GLuint> diffuse_texture;
    std::shared_ptr<GLuint> utility_texture;
	std::vector <Bone> bones;

	glModel(GLResourcesManager &resources_manager)
	{
		glGenVertexArrays(1, &jal_mesh.VAO);
		glGenBuffers(1, &jal_mesh.VBO);
		glGenBuffers(1, &jal_mesh.VBO_BONES);
		glGenBuffers(1, &jal_mesh.VBO_BONES_IDX);
	}

	glModel(std::string FileName)
	{
		glGenVertexArrays(1, &jal_mesh.VAO);
		glGenBuffers(1, &jal_mesh.VBO);
		glGenBuffers(1, &jal_mesh.VBO_BONES);
		glGenBuffers(1, &jal_mesh.VBO_BONES_IDX);
		LoadAll(FileName);
	}

	glModel(std::string FileName,std::vector <std::shared_ptr<Animation> > &animations)
	{
		glGenVertexArrays(1, &jal_mesh.VAO);
		glGenBuffers(1, &jal_mesh.VBO);
		glGenBuffers(1, &jal_mesh.VBO_BONES);
		glGenBuffers(1, &jal_mesh.VBO_BONES_IDX);
		LoadAll(FileName,animations);
	}



	const glm::mat4 &ModelMatrix()
	{
		return model;
	}


	~glModel()
	{

		glDeleteVertexArrays(1, &jal_mesh.VAO);
		glDeleteBuffers(1, &jal_mesh.VBO);
		glDeleteBuffers(1, &jal_mesh.VBO_BONES);
		glDeleteBuffers(1, &jal_mesh.VBO_BONES_IDX);
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

#endif