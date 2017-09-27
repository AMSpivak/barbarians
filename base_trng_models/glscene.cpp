#include "glscene.h"
#include <fstream>
#include <iostream>
//---------------------------------------------------------------

void glModel::LoadModel(std::string FileName)
{
	LoadVertexArray(FileName, VBO, VBO_BONES, VBO_BONES_IDX, vertexcount);

	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_BONES_IDX);
    //glBufferData(GL_ARRAY_BUFFER, Model->indexNum*4*sizeof(GLint), VertexBoneArray, GL_STATIC_DRAW);

    glVertexAttribIPointer(3, 4, GL_INT, 4 * sizeof(GL_INT), (GLvoid*)0);
    glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BONES);
    //glBufferData(GL_ARRAY_BUFFER, Model->indexNum*4*sizeof(GLfloat), VertexWeigthArray, GL_STATIC_DRAW);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(4);




    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0);
}

void glModel::LoadModelBones(std::string FileName)
{
	LoadBonesArray(FileName,bones,bonescount);

}

void glModel::Draw()
{
	if(vertexcount > 2)
	{
	    glBindVertexArray(VAO);
	    glDrawArrays(GL_TRIANGLES, 0, vertexcount);
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
	glBindTexture(GL_TEXTURE_2D, diffuse_texture);

	glUniform1i(glGetUniformLocation(shaderProgram, "UtilityTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, utility_texture);
    

	glUniformMatrix4fv(boneLoc, bones.size(), GL_FALSE, glm::value_ptr(animation.frames[now_frame].bones[0]));
    Draw();
}

void glModel::Draw(GLuint shaderProgram, int now_frame)
{
	Draw(shaderProgram, *animation ,now_frame);
}

void glModel::AttachAnimation(std::vector <std::shared_ptr<Animation> > &animations, std::string Filename)
{
	animations.emplace_back(std::make_shared<Animation>());
	animations.back()->LoadAnimation(Filename,bones);
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
	jal_name = path + tmp_str;
	getline(modelfile, tmp_str);
	jub_name = path + tmp_str;
	getline(modelfile, tmp_str);
	png_name = path + tmp_str;
	modelfile >> parent_idx >> parent_bone;
	std::cout<<jal_name<<"\n"<<jub_name<<"\n"<<png_name<<"\n"<<"!"<<parent_idx<<"!"<<parent_bone<<"\n";
	modelfile.close();

	LoadModel(jal_name);
	LoadTexture(png_name, diffuse_texture);
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
	jal_name = path + tmp_str;
	getline(modelfile, tmp_str);
	jub_name = path + tmp_str;
	getline(modelfile, tmp_str);
	png_name = path + tmp_str;
    getline(modelfile, tmp_str);
    png_utility_name = path + tmp_str;
	modelfile >> parent_idx >> parent_bone>>frames_name;
    std::cout<<jal_name<<"\n"<<jub_name<<"\n"<<png_name<<"\n"<<png_utility_name<<"\n"<<"!"<<parent_idx<<"!"<<parent_bone<<"\n"<<frames_name<<"\n";

	modelfile.close();

	LoadModel(jal_name);
	LoadTexture(png_name, diffuse_texture);
    LoadTexture(png_utility_name, utility_texture);
	LoadModelBones(jub_name);
	if(frames_name.compare("")) AttachAnimation(animations,path + frames_name);


}
//---------------------------------------------------------------
