#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
class Camera
{
public:
	Camera();
private:
	glm::mat4 proj;
	glm::mat4 view;

	glm::vec3 pos = glm::vec3(0.f,0.f,10.f);
	glm::vec3 forward = glm::vec3(0,0,-1);
	glm::vec3 right = glm::vec3(1,0,0);
	glm::vec3 up = glm::vec3(0,1,0);
	glm::vec3 worldUp = glm::vec3(0, 1, 0);
public:
	void Update();
	void CreateProj(float fov, float aspect, float znear, float zfar);
	void Strafe(float value);
	void MoveForward(float value);
	void MoveUp(float value);
	void RotateY(float value);
	void Pitch(float value);
	void RotateCamera(float pitch, float yaw);

	glm::mat4 GetProj() { return proj; }
	glm::mat4 GetView() { return view; }
	glm::vec3 GetCameraPosition() { return pos; }
	glm::vec3 GetCameraForward() { return forward; }
};

