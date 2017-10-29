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
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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







glm::mat4 LoadMatrix(std::ifstream &source)
{
	glm::mat4 mat;
	if(source.is_open()) source >> mat;
	return  mat;
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

void LoadTexture(const std::string &FileName,GLuint &texture)
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
	std::cout<<"Load texture: "<<FileName<<"\n";
}


