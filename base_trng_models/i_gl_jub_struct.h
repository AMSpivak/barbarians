#ifndef GL_JUB_STRUCT
#define GL_JUB_STRUCT
#include <GL/glew.h>

struct IGlJubStruct
{
    std::vector <Bone> bones;
	int bonescount;
};
#endif