#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
struct BoundingBox
{
	static const unsigned char CORNER_COUNT = 4;
	glm::vec3 Center;
	glm::vec3 Extents;
	glm::vec3 min;
	glm::vec3 max;
};

struct Ray
{
	//Ray() {}
	Ray(glm::vec3 origin, glm::vec3 direction) : Origin(origin), Direction(direction) {}
	glm::vec3 Origin;
	glm::vec3 Direction;
};

class Bounds
{
public:
	static BoundingBox GetBounds(const std::vector<glm::vec3>& vertices);

	static bool intersect(const Ray& ray, const BoundingBox& bounds);
};

