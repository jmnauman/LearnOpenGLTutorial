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

glm::mat4 FlyCamera::getManualView() const
{
	// As an exercise, this constructs the LookAt matrix manually instead of relying on GLM.
	// One part about this might be crucial: up is not necessarily perpendicular to both right and forward.
	// The matrix we were given assumes that they are perpendicular.

	// Create the identity matrix
	glm::mat4 m(1.f);
	// translate by the negated camera position
	m = glm::translate(m, -cameraPos);
	// find a right vector perpendicular up vector given front and right
	glm::vec3 right = glm::normalize(glm::cross(cameraFront, up));
	glm::vec3 perpendicularUp = glm::normalize(glm::cross(right, cameraFront));
	// rotate by the matrix given in the notes. Note that we could probably convert these to vec4 use those as the arguments, and transpose
	// We fill in the columns first.
	// Z is flipped, because our camera's positive Z is looking down OpenGL's -Z... I think. I'm having trouble with that part.
	// but, glm::lookAt will negate the view direction.
	glm::mat4 rot(right.x, perpendicularUp.x, -cameraFront.x, 0.f,
		right.y, perpendicularUp.y, -cameraFront.y, 0.f,
		right.z, perpendicularUp.z, -cameraFront.z, 0.f,
		0.f, 0.f, 0.f, 1.f);
	return rot * m;
}

void FlyCamera::adjustLook(float dx, float dy)
{
	yaw += dx * yawSensitivity;
	pitch -= dy * pitchSensitivity;
	pitch = glm::clamp(pitch, -89.f, 89.f);
	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);
	cameraFront.x = cos(yawRad) * cos(pitchRad);
	cameraFront.y = sin(pitchRad);
	cameraFront.z = sin(yawRad) * cos(pitchRad);
	cameraFront = glm::normalize(cameraFront);
}

void FlyCamera::moveForward(const float deltaTime)
{
	cameraPos += deltaTime *cameraSpeed * cameraFront;
}

void FlyCamera::moveBackward(float deltaTime)
{
	cameraPos -= deltaTime * cameraSpeed * cameraFront;
}

void FlyCamera::moveLeft(float deltaTime)
{
	cameraPos -= deltaTime * cameraSpeed * glm::normalize(glm::cross(cameraFront, up));
}

void FlyCamera::moveRight(float deltaTime)
{
	cameraPos += deltaTime * cameraSpeed * glm::normalize(glm::cross(cameraFront, up));
}

void FlyCamera::moveUp(float deltaTime)
{
	cameraPos += deltaTime * cameraSpeed * up;
}

void FlyCamera::moveDown(float deltaTime)
{
	cameraPos -= deltaTime * cameraSpeed * up;
}

void FlyCamera::zoom(float offset)
{
	fov -= offset;
	fov = glm::clamp(fov, 1.f, 80.f);
}

glm::vec3 FlyCamera::getPos() const
{
	return cameraPos;
}

glm::vec3 FlyCamera::getFront() const
{
	return cameraFront;
}
