#pragma once

#include <glm/glm.hpp>
#include "helpers.h"

class FlyCamera
{
public:
	FlyCamera() = default;
	FlyCamera(float aspect);

	glm::mat4 getProj() const;
	glm::mat4 getView() const;
	void adjustLook(float dx, float dy);
	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	void zoom(float offset);

private:
	float cameraSpeed = 2.5f;
	float fov = 60.f;
	float pitch = 0.f;
	float yaw = -90.f;
	float aspect = 1;
	float nearClip = 0.1f;
	float farClip = 100.f;
	float yawSensitivity = 0.5f;
	float pitchSensitivity = 0.5f;

	glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
	glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
};

