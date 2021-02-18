#pragma once

struct Ac_BoxColliderComponent
{
	glm::vec3 Min;
	glm::vec3 Max;
};

struct Ac_SphereColliderComponent
{
	glm::vec3 Pos;
	float Radius;
};

struct Ac_PlaneColliderComponent
{
	glm::vec3 Normal;
	float Distance;
};