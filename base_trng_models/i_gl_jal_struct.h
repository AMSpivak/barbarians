#ifndef GL_JAL_STRUCT
#define GL_JAL_STRUCT
#include <GL/glew.h>
struct IGlJalStruct
{
    GLuint VBO, VBO_BONES, VBO_BONES_IDX, VAO;
	int vertexcount;
};
#endif