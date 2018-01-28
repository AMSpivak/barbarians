#include "gl_jal_mesh_atlas.h"
#include "glresourses.h"
#include <iostream>

GLJalMeshAtlas::GLJalMeshAtlas(const std::string & ResourseFolder):GLAtlas<IGlJalStruct>(ResourseFolder)
{}


void GLJalMeshAtlas::FillResource(std::string filename,IGlJalStruct * resource)
{
    glGenVertexArrays(1, &resource->VAO);
	glGenBuffers(1, &resource->VBO);
	glGenBuffers(1, &resource->VBO_BONES);
	glGenBuffers(1, &resource->VBO_BONES_IDX);
    //LoadTexture(filename,*(resource));
    LoadVertexArray(filename, resource->VBO, resource->VBO_BONES, resource->VBO_BONES_IDX, resource->vertexcount);

	glBindVertexArray(resource->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, resource->VBO);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, resource->VBO_BONES_IDX);

    glVertexAttribIPointer(4, 4, GL_INT, 4 * sizeof(GL_INT), (GLvoid*)0);
    glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, resource->VBO_BONES);

    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);




    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0);

}


void GLJalMeshAtlas::EmptyResource(IGlJalStruct * resource)
{
    glDeleteVertexArrays(1, &resource->VAO);
    glDeleteBuffers(1, &resource->VBO);
    glDeleteBuffers(1, &resource->VBO_BONES);
    glDeleteBuffers(1, &resource->VBO_BONES_IDX);
}