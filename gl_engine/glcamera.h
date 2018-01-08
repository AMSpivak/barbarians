#ifndef GL_ENGINE_CAMERA
#define GL_ENGINE_CAMERA

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class glCamera
{
	glm::mat4 view;
    glm::mat4 projection;
	glm::mat4 full_matrix;
public:
	const glm::mat4 &CameraMatrix()
	{
		//full_matrix =  projection * view;
		//return projection * view;
		return full_matrix;
        
	}

	const glm::mat4 &CameraViewMatrix()
	{
		return view;
	}

	const glm::mat4 &CameraProjectionMatrix()
	{
		return projection;
	}

	void SetCameraLocation(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
	{
		view = glm::lookAt(position,target,up);
        full_matrix =  projection * view;
	}

	void SetCameraLens(float FOV, float aspect, float near, float far)
	{
		projection = glm::perspective(glm::radians(FOV), aspect, near, far);
        full_matrix =  projection * view;
	}
	void SetCameraLens_Orto(float x1, float x2,float y1, float y2, float near, float far)
	{
		projection = glm::ortho(x1, x2, y1, y2, near, far);
        full_matrix =  projection * view;
	}
};

#endif