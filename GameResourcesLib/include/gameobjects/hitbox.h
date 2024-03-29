#ifndef HITBOX_H
#define HITBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

struct Hitbox {
	struct Axis {
		glm::vec3 x;
		glm::vec3 y;
		glm::vec3 z;

		Axis() :x(glm::vec3(1.0f, 0.0f, 0.0f)), y(glm::vec3(0.0f, 1.0f, 0.0f)), z(glm::vec3(0.0f, 0.0f, 1.0f)) {}
	};
	struct MoveValues {
		float xMove[2];
		float yMove[2];
		float zMove[2];

		MoveValues() :xMove{ 0.0f,0.0f }, yMove{ 0.0f,0.0f }, zMove{ 0.0f,0.0f } {}
	};
	struct IndexGroup {
		std::set<int> idx;
		int baseIdx;
		int status;
		enum Status { OPEN, CLOSED, IN_USE };

		IndexGroup(int baseIdx_) :baseIdx(baseIdx_), status(OPEN), idx(std::set<int>()) {}
	};

	std::string name;
	std::vector<int> controlPointsIdx;

	Axis localAxis;
	glm::vec3 basicVertices[8];
	glm::vec3 transformVertices[8];
	glm::vec3 basicNormals[6];
	glm::vec3 transformNormals[6];
	int jointIdx;
	std::vector<unsigned> indices;

	double damageMultiplier;

	Hitbox();

	void deepCopy(const Hitbox& hitbox);

};

typedef std::shared_ptr<Hitbox> HitboxPtr;
typedef std::map<int, HitboxPtr> HitboxMap;
typedef std::shared_ptr<HitboxMap> HitboxMapPtr;

struct HitboxSet{
    HitboxPtr mainHitbox;
    HitboxMapPtr hitboxes;
};

#endif //HITBOX_H
