#ifndef GL_JAL_STRUCT
#define GL_JAL_STRUCT
#include <GL/glew.h>
struct IGlJalStruct
{
    GLuint VBO, VBO_BONES, VBO_BONES_IDX, VAO;
	int vertexcount;

    IGlJalStruct()
    {
        /*glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VBO_BONES);
		glGenBuffers(1, &VBO_BONES_IDX);*/
    }
    
    ~IGlJalStruct()
    {
        /*glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &VBO_BONES);
		glDeleteBuffers(1, &VBO_BONES_IDX);*/
    }
};
#endif