#include "FlyCamera.h"

FlyCamera::FlyCamera(float aspect)
{
	this->aspect = aspect;
}

glm::mat4 FlyCamera::getProj() const
{
	return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
}

glm::mat4 FlyCamera::getView() const
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, up);
}

void FlyCamera::adjustLook(float dx, float dy)
{
	yaw += dx * yawSensitivity;
	pitch -= dy * pitchSensitivity;
	glm::clamp(pitch, -89.f, 89.f);
	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);
	cameraFront.x = cos(yawRad) * cos(pitchRad);
	cameraFront.y = sin(pitchRad);
	cameraFront.z = sin(yawRad) * cos(pitchRad);
	cameraFront = glm::normalize(cameraFront);
}

void FlyCamera::moveForward(const float deltaTime)
{
	glm::vec3 moveDir = glm::normalize(glm::vec3(cameraFront.x, 0.f, cameraFront.z));
	cameraPos += deltaTime * cameraSpeed * moveDir;
}

void FlyCamera::moveBackward(float deltaTime)
{
	glm::vec3 moveDir = glm::normalize(glm::vec3(cameraFront.x, 0.f, cameraFront.z));
	cameraPos -= deltaTime * cameraSpeed * moveDir;
}

void FlyCamera::moveLeft(float deltaTime)
{
	cameraPos -= deltaTime * cameraSpeed * glm::normalize(glm::cross(cameraFront, up));
}

void FlyCamera::moveRight(float deltaTime)
{
	cameraPos += deltaTime * cameraSpeed * glm::normalize(glm::cross(cameraFront, up));
}

void FlyCamera::zoom(float offset)
{
	fov -= offset;
	fov = glm::clamp(fov, 1.f, 80.f);
}
