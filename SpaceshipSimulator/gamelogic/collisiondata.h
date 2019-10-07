#pragma once

#include <glm/vec3.hpp>

struct CollisionData
{
	enum ColType { hitbox_crash, laser_shot };

	glm::vec3 impactVector;

	float collisionEnergy;

	GameObject& internalCollisionObj;
	GameObject& externalCollisionObj;

	ColType collisionType;

	CollisionData(GameObject& internalObject, GameObject& externalObject)
		: internalCollisionObj(internalObject)
		, externalCollisionObj(externalObject)
	{}
};