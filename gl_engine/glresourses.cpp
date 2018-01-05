#include "glresourses.h"
#include <sstream>
#include <iostream>
#include <fstream>


void Fit_Matrix(glm::mat4 &matrix,float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3)
{
	float mScale_Matrix[16] = {0};

	//Matrix.setIdentityM(mScale_Matrix,0);
	mScale_Matrix[0]=x0;
	mScale_Matrix[1]=y0;
	mScale_Matrix[2]=0.0f;
	mScale_Matrix[3]=1.0f;
	mScale_Matrix[4]=x1;
	mScale_Matrix[5]=y1;
	mScale_Matrix[6]=0.0f;
	mScale_Matrix[7]=1.0f;
	mScale_Matrix[8]=x3;
	mScale_Matrix[9]=y3;
	mScale_Matrix[10]=0.0f;
	mScale_Matrix[11]=1.0f;
	mScale_Matrix[12]=x2;
	mScale_Matrix[13]=y2;
	mScale_Matrix[14]=0.0f;
	mScale_Matrix[15]=1.0f;

	matrix = glm::make_mat4(mScale_Matrix);

}


void renderSprite(GLuint current_shader,
	float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3,
	const glm::vec4 & corrector_v,
	const GLuint * texture 
)
{


    static unsigned int quadVAO = 0;
    static unsigned int quadVBO;

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
             // R, G, B, A

			 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f,

			 0.0f, 1.0f, 0.0f, 0.0f,
			 1.0f, 1.0f,

			 0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f, 0.0f,

			 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 0.0f
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
	}
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(current_shader);

	glm::mat4 texture_m = glm::mat4(1.0f);
	//texture_m = glm::scale(model_m,glm::vec3(1.0f,(float)width/height,1.0f));
	glm::mat4 position_m = glm::mat4(1.0f);
	Fit_Matrix(position_m,
		x0,y0,x1,y1,x2,y2,x3,y3
	);
	GLuint position_u  = glGetUniformLocation(current_shader, "DrawMatrix");
	glUniformMatrix4fv(position_u, 1, GL_FALSE, glm::value_ptr(position_m));

	GLuint texture_u  = glGetUniformLocation(current_shader, "SpriteMatrix");
	glUniformMatrix4fv(texture_u, 1, GL_FALSE, glm::value_ptr(texture_m));

	
	GLuint corrector_u  = glGetUniformLocation(current_shader, "corrector");
	glUniform4fv(corrector_u, 1, glm::value_ptr(corrector_v));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void renderSpriteDepth(GLuint current_shader, GLuint depthmap, float sprite_depth,
	float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3,
	const glm::vec4 & corrector_v,
	const GLuint * texture 
)
{


    static unsigned int quadVAO = 0;
    static unsigned int quadVBO;

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
             // R, G, B, A

			 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f,

			 0.0f, 1.0f, 0.0f, 0.0f,
			 1.0f, 1.0f,

			 0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f, 0.0f,

			 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 0.0f
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
	}
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(current_shader);

	glm::mat4 texture_m = glm::mat4(1.0f);
	//texture_m = glm::scale(model_m,glm::vec3(1.0f,(float)width/height,1.0f));
	glm::mat4 position_m = glm::mat4(1.0f);
	Fit_Matrix(position_m,
		x0,y0,x1,y1,x2,y2,x3,y3
	);
	GLuint position_u  = glGetUniformLocation(current_shader, "DrawMatrix");
	glUniformMatrix4fv(position_u, 1, GL_FALSE, glm::value_ptr(position_m));

	GLuint texture_u  = glGetUniformLocation(current_shader, "SpriteMatrix");
	glUniformMatrix4fv(texture_u, 1, GL_FALSE, glm::value_ptr(texture_m));

	
	GLuint corrector_u  = glGetUniformLocation(current_shader, "corrector");
	glUniform4fv(corrector_u, 1, glm::value_ptr(corrector_v));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glUniform1i(glGetUniformLocation(current_shader, "DepthMap"), 1);
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D, depthmap);

	glActiveTexture(GL_TEXTURE0);


	GLint shader_depth = glGetUniformLocation(current_shader, "Depth");
   	glUniform1f(shader_depth, sprite_depth);


    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void renderQuad()
{
    static unsigned int quadVAO = 0;
    static unsigned int quadVBO;

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


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
	if(FileName.substr(FileName.find_last_of(".")+1) == "cub")
	{
		LoadCubemap(FileName,texture);
		return;
	}
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

void LoadCubemap(const std::string FileName,GLuint &texture)
{
	size_t pos = FileName.rfind("/");
	
	std::ifstream modelfile;
	modelfile.open(FileName);

	std::string path = pos == std::string::npos ? "" : FileName;
	if(pos != std::string::npos)
	{
		path.erase(pos+1,std::string::npos);
	}
	std::string tmp_string = "";
	std::vector<std::string> faces;
	std::ifstream texture_descriptor_file;
	texture_descriptor_file.open(FileName);
	while(faces.size()<6 && !texture_descriptor_file.eof())
	{
		getline(texture_descriptor_file, tmp_string);
		faces.push_back(tmp_string);
	}
	if(faces.size()<6)
	{
		std::cout << "Wrong cubemap descriptor: " <<FileName;
	}

	texture_descriptor_file.close();


	glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture); 

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    int tex_width, tex_height;

 	for (unsigned int i = 0; i < faces.size(); i++)
    {
		std::string file = path+faces[i];
        unsigned char *data = SOIL_load_image(file.c_str(), &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
            );
            SOIL_free_image_data(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << file << std::endl;
            SOIL_free_image_data(data);
        }
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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
