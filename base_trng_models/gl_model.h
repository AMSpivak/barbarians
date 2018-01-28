#ifndef GL_MODEL
#define GL_MODEL
#include <GL/glew.h>

#include <string>
#include <vector>
#include <memory>
#include "glresourses.h"
#include <iostream>
#include "i_gl_jal_struct.h"
#include "i_gl_jub_struct.h"


#include "gl_resources_manager.h"

class glModel
{

	std::shared_ptr<IGlJalStruct> jal_mesh;

	std::shared_ptr<Animation> animation;
	std::string name;
	//GLResourcesManager &m_resources_manager;

public:
	int parent_idx;
	int parent_bone;

	glm::mat4 model;
	std::shared_ptr<GLuint> diffuse_texture;
    std::shared_ptr<GLuint> utility_texture;
    std::shared_ptr<GLuint> normal_texture;
	std::shared_ptr<IGlJubStruct> jub_bones;

	glModel(GLResourcesManager &resources_manager)
	{}

	glModel(std::string FileName)
	{
		LoadAll(FileName);
	}

	glModel(std::string FileName,std::vector <std::shared_ptr<Animation> > &animations)
	{
		LoadAll(FileName,animations);
	}



	const glm::mat4 &ModelMatrix()
	{
		return model;
	}


	~glModel()
	{}





	void LoadModelBones(std::string FileName);
	void LoadAll(std::string FileName);
	void LoadAll(std::string FileName,std::vector <std::shared_ptr<Animation> > &animations);
	void Draw();
	void Draw(GLuint shaderProgram, Animation &animation, int now_frame);
	void Draw(GLuint shaderProgram, int now_frame);
	void AttachAnimation(std::vector <std::shared_ptr<Animation> > &animations, std::string Filename);
};

#endif