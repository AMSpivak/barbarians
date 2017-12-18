#include "gl_model.h"
#include <fstream>
#include <iostream>



void glModel::LoadModelBones(std::string FileName)
{
	IGlJubStruct * bone_ptr = jub_bones.get();
	LoadBonesArray(FileName,bone_ptr->bones,bone_ptr->bonescount);

}

void glModel::Draw()
{

	IGlJalStruct * mesh_ptr = jal_mesh.get();
	
	if(mesh_ptr->vertexcount > 2)
	{
	    glBindVertexArray(mesh_ptr->VAO);
	    glDrawArrays(GL_TRIANGLES, 0, mesh_ptr->vertexcount);
	    glBindVertexArray(0);
	}
}

void glModel::Draw(GLuint shaderProgram, Animation &animation, int now_frame)
{
	//glUseProgram(shader);
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int boneLoc  = glGetUniformLocation(shaderProgram, "u_BoneMatrices");


	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *diffuse_texture.get());

	glUniform1i(glGetUniformLocation(shaderProgram, "UtilityTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *utility_texture.get());
    

	glUniformMatrix4fv(boneLoc, jub_bones.get()->bones.size(), GL_FALSE, glm::value_ptr(animation.frames[now_frame].bones[0]));
    Draw();
}

void glModel::Draw(GLuint shaderProgram, int now_frame)
{
	Draw(shaderProgram, *animation ,now_frame);
}

void glModel::AttachAnimation(std::vector <std::shared_ptr<Animation> > &animations, std::string Filename)
{
	animations.emplace_back(std::make_shared<Animation>());
	animations.back()->LoadAnimation(Filename,jub_bones.get()->bones);
	animation = animations.back();
}

void glModel::LoadAll(std::string FileName)
{
	size_t pos = FileName.rfind("/");
	std::ifstream modelfile;
	modelfile.open(FileName);

	std::string path = pos == std::string::npos ? "" : FileName.erase(pos+1,std::string::npos);
	std::string tmp_str;
	std::string jal_name;
	std::string jub_name;
	std::string png_name;
	std::string frames_name = "";

	getline(modelfile, tmp_str);
	jal_name = /*path + */tmp_str;
	getline(modelfile, tmp_str);
	jub_name = path + tmp_str;
	getline(modelfile, tmp_str);
	png_name = /*path + */tmp_str;
	modelfile >> parent_idx >> parent_bone;
	std::cout<<jal_name<<"\n"<<jub_name<<"\n"<<png_name<<"\n"<<"!"<<parent_idx<<"!"<<parent_bone<<"\n";
	modelfile.close();
	GLResourcesManager * resources = GetResourceManager();
	//LoadModel(jal_name);
	name = jal_name;	
	jal_mesh = resources->m_mesh_atlas.Assign(jal_name);
	diffuse_texture = resources->m_texture_atlas.Assign(png_name);
	LoadModelBones(jub_name);


}
void glModel::LoadAll(std::string FileName,std::vector <std::shared_ptr<Animation> > &animations)
{
	size_t pos = FileName.rfind("/");
	std::ifstream modelfile;
	modelfile.open(FileName);

	std::string path = pos == std::string::npos ? "" : FileName.erase(pos+1,std::string::npos);
	std::string tmp_str;
	std::string jal_name;
	std::string jub_name;
	std::string png_name;
    std::string png_utility_name;
	std::string frames_name = "";

	getline(modelfile, tmp_str);
	jal_name = /*path + */tmp_str;
	getline(modelfile, tmp_str);
	jub_name = path + tmp_str;
	getline(modelfile, tmp_str);
	png_name = /*path + */tmp_str;
    getline(modelfile, tmp_str);
    png_utility_name = /*path + */tmp_str;
	modelfile >> parent_idx >> parent_bone>>frames_name;
    //std::cout<<jal_name<<"\n"<<jub_name<<"\n"<<png_name<<"\n"<<png_utility_name<<"\n"<<"!"<<parent_idx<<"!"<<parent_bone<<"\n"<<frames_name<<"\n";

	modelfile.close();
	name = jal_name;

	GLResourcesManager * resources = GetResourceManager();
	//LoadModel(jal_name);
	jal_mesh = resources->m_mesh_atlas.Assign(jal_name);
	diffuse_texture = resources->m_texture_atlas.Assign(png_name);
	utility_texture = resources->m_texture_atlas.Assign(png_utility_name);
	
	//LoadTexture(png_name, diffuse_texture);
    //LoadTexture(png_utility_name, utility_texture);
	LoadModelBones(jub_name);
	if(frames_name.compare("")) AttachAnimation(animations,path + frames_name);


}