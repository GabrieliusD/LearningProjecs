#include "Camera.h"

Camera::Camera()
{
}

void Camera::Update()
{
	glm::vec3 newPos = pos;
	glm::vec3 newforward = glm::normalize((newPos + forward) - newPos);
	glm::vec3 newright = glm::normalize(glm::cross(glm::normalize(forward), worldUp));
	glm::vec3 newup = glm::cross(right, forward);

//	glm::mat4 orentation(
//		glm::vec4(right.x, right.y,right.z, 0),
//		glm::vec4(up.x, up.y,up.z, 0),
//		glm::vec4(-forward.x, -forward.y, -forward.z, 0),
//		glm::vec4(-glm::dot(right,pos), -glm::dot(up, pos), -glm::dot(forward, pos), 1)
//	);
	glm::mat4 Result(1);
	Result[0][0] = newright.x;
	Result[1][0] = newright.y;
	Result[2][0] = newright.z;
	Result[0][1] = newup.x;
	Result[1][1] = newup.y;
	Result[2][1] = newup.z;
	Result[0][2] = -newforward.x;
	Result[1][2] = -newforward.y;
	Result[2][2] = -newforward.z;
	Result[3][0] = -glm::dot(newright, newPos);
	Result[3][1] = -glm::dot(newup, newPos);
	Result[3][2] = -glm::dot(newforward, newPos);

	view = glm::lookAt(pos, pos + forward, glm::vec3(0, 1, 0));
	
	//view = Result;
}

void Camera::CreateProj(float fov, float aspect, float znear, float zfar)
{
	proj = glm::perspective(fov, aspect, znear, zfar);
}

void Camera::Strafe(float value)
{
	right = glm::normalize(glm::cross(forward,up));
	pos += value * right;
}

void Camera::MoveForward(float value)
{
	//std::cout <<" " << forward.x << " " << forward.y << " "<< forward.z << std::endl;
	pos += value * forward;
}

void Camera::MoveUp(float value)
{
	pos += value * up;
}

void Camera::RotateY(float yaw)
{
	
}

void Camera::Pitch(float pitch)
{

}

void Camera::RotateCamera(float pitch, float yaw)
{

	glm::vec3 cameraDir(1.0f);
	cameraDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDir.y = sin(glm::radians(pitch));

	forward = glm::normalize(cameraDir);
}
