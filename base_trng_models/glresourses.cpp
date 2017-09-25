#include "glresourses.h"
#include <sstream>
#include <iostream>
#include <fstream>

std::istream& operator>> ( std::istream& is, glm::mat4& mat)
{
	float marray[16];
	//for(int i = 0; i< 16; i++)
	//is >> marray[i];
	is >> marray[0]	>> marray[4]>> marray[8]>> marray[12]
	>> marray[1]	>> marray[5]>> marray[9]>> marray[13]
	>> marray[2]	>> marray[6]>> marray[10]>> marray[14]
	>> marray[3]	>> marray[7]>> marray[11]>> marray[15]	;
	mat = glm::make_mat4(marray);
	return is;
}


std::istream& operator >> ( std::istream& is, Bone& bone)
{
	is >> bone.name >> bone.parent >> bone.matrix;
    return is;
}


std::string readShaderFile(std::string FileName)
{
	std::string ShaderString = "";
	std::ifstream shaderFile;
	shaderFile.open(FileName);
	while(!shaderFile.eof())
	{
		std::string tempholder;
		getline(shaderFile, tempholder);
		ShaderString.append(tempholder);
		ShaderString.append("\n");
	}
	shaderFile.close();

	return ShaderString;
}

GLuint LoadShader(std::string FileName,GLenum shaderType)
{
	std::string shader_text = readShaderFile(FileName);
	GLchar const * ShaderSource = shader_text.c_str();

    GLuint Shader = glCreateShader(shaderType);
    glShaderSource(Shader, 1, &ShaderSource, NULL);
    glCompileShader(Shader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(Shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::"
		<<(shaderType == GL_VERTEX_SHADER ? "VERTEX":"FRAGMENT")<<"::COMPILATION_FAILED\n"
		<< infoLog << std::endl;
		return -1;
    }
	return Shader;
}

GLuint LoadshaderProgram(std::string FileNameVS,std::string FileNameFS)
{
	GLuint Shaders[2];
    /*GLuint vertexShader*/
	Shaders[0] = LoadShader(FileNameVS,GL_VERTEX_SHADER);
    /*GLuint fragmentShader*/
	Shaders[1] =  LoadShader(FileNameFS,GL_FRAGMENT_SHADER);//glCreateShader(GL_FRAGMENT_SHADER);
    // Link shaders
    GLuint shaderProgram = LinkShaderProgram(Shaders,2);
    EmptyShaders(Shaders,2);
	return shaderProgram;
}



void LoadVertexArray(std::string FileName,GLuint &VBO, GLuint &VBO_BONES, GLuint &VBO_BONES_IDX, int &vertex_count)
{

	std::string tmp_string = "";
	std::ifstream ModelFile;
	ModelFile.open(FileName);
	if(ModelFile.is_open())
	{
		//ModelFile >> tmp_string;
		getline(ModelFile, tmp_string);
		//std::cout << tmp_string;
		getline(ModelFile, tmp_string);
		//std::cout << tmp_string;
		ModelFile >> tmp_string;
		//std::cout << tmp_string;
		ModelFile >> vertex_count;
		vertex_count *= 3;
		std::cout << vertex_count;
		//std::cout << "\n";
		getline(ModelFile, tmp_string);
		getline(ModelFile, tmp_string);
		//std::cout << tmp_string<< "\n";

		GLfloat * vertices = new GLfloat[vertex_count*8];
		GLfloat * bone_weight = new GLfloat[vertex_count*4];
		GLint * bone_indexes = new GLint[vertex_count*4];

		for(int i_v = 0; i_v < vertex_count; i_v++)
		{
			for(int i = 0; i < 8; i++)
			{
				ModelFile >> vertices[i + i_v * 8];
			}

			getline(ModelFile, tmp_string);

			getline(ModelFile, tmp_string);


			std::istringstream iss(tmp_string);

			iss.width(10);
			iss.precision(8);

			for(int i = 0; i < 4; i++)
				{
					bone_indexes[i_v * 4 + i] = 0;
					bone_weight[i + i_v * 4] = 0.0f;
				}

			int ndx = 0;
			while (!(iss.eof()))
			{
				iss >> bone_indexes[i_v * 4 + ndx] >> bone_weight[(ndx) + i_v * 4];
				ndx++;
			}
		}

		ModelFile.close();





		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * 8* sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_BONES);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * 4* sizeof(GLfloat), bone_weight, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_BONES_IDX);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * 4* sizeof(GLint), bone_indexes, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete [] vertices;
		delete [] bone_weight;
		delete [] bone_indexes;
	}
}



glm::mat4 LoadMatrix(std::ifstream &source)
{
	glm::mat4 mat;
	if(source.is_open()) source >> mat;
	return  mat;
}

Bone LoadBone(std::ifstream &source)
{
	Bone bone;
	if(source.is_open()) source >> bone;
	return  bone;
}


void LoadBonesArray(std::string FileName,std::vector <Bone> &bones,int &bones_count)
{
	std::string tmp_string = "";
	std::ifstream ModelFile;
	ModelFile.open(FileName);
	getline(ModelFile, tmp_string);
	ModelFile>>tmp_string>>bones_count>>tmp_string;
	//getline(ModelFile, tmp_string);
	//getline(ModelFile, tmp_string);
	//getline(ModelFile, tmp_string);
	Bone bone;
	bones.clear();

	for(int i = 0; i < bones_count; i++)
	{
		ModelFile>>bone;
		bones.push_back(bone);
	}
	ModelFile.close();
}
GLuint LinkShaderProgram(GLuint * shaders, int shaders_count)
{
	GLuint shaderProgram = glCreateProgram();
	for(int i = 0; i <shaders_count; i++) glAttachShader(shaderProgram, shaders[i]);

    glLinkProgram(shaderProgram);
    // Check for linking errors
	GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return -1;
    }
	return shaderProgram;
}

void EmptyShaders(GLuint * shaders, int shaders_count)
{
	for(int i = 0; i <shaders_count; i++) glDeleteShader(shaders[i]);
}

void LoadTexture(std::string FileName,GLuint &texture)
{
	glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    int tex_width, tex_height;
    unsigned char* image = SOIL_load_image(FileName.c_str(), &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Animation::LoadAnimation(std::string FileName, std::vector <Bone> &bones)
{
	int bon_count = 0;
	std::string tmp_string = "";
	std::ifstream ModelFile;
	ModelFile.open(FileName);
	getline(ModelFile, tmp_string);
	ModelFile>>tmp_string>>bon_count>>tmp_string>>tmp_string>>framescount;
	frames.clear();
	int current_frame;
	glm::mat4 tmp_matrix;
	for(int i_frame = 0; i_frame < framescount; i_frame++)
	{
		ModelFile>>tmp_string>>current_frame;
		AnimationFrame a_frame;
		for(int i = 0; i < bon_count; i++)
		{

			ModelFile>>tmp_string>>tmp_matrix;
			//std::cout<<tmp_string<<"\n";
			a_frame.bones.push_back( tmp_matrix  * glm::inverse(bones[i].matrix));

		}
		frames.push_back(a_frame);
	}
	ModelFile.close();
}
