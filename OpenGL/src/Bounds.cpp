#include "Bounds.h"

BoundingBox Bounds::GetBounds(const std::vector<glm::vec3>& vertices)
{
	Ray ray();
	glm::vec3 vMin((float)INT_MAX, (float)INT_MAX, (float)INT_MAX);
	glm::vec3 vMax(-(float)INT_MAX, -(float)INT_MAX, -(float)INT_MAX);
	
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		
		vMax = glm::max(vMax, vertices[i]);
		vMin = glm::min(vMin, vertices[i]);

	}

	BoundingBox bounds;
	bounds.Center = 0.5f * (vMin + vMax);
	bounds.Extents = 0.5f * (vMax - vMin);
	bounds.min = vMin;
	bounds.max = vMax;
	
	return bounds;
}

void swap(float &n1, float &n2)
{
	float temp = n1;
	n1 = n2;
	n2 = temp;
}

bool Bounds::intersect(const Ray& ray, const BoundingBox& bounds)
{
	float tMin = (bounds.min.x - ray.Origin.x) / ray.Direction.x;
	float tMax = (bounds.max.x - ray.Origin.x) / ray.Direction.x;

	if (tMin > tMax) swap(tMin, tMax);

	float tymin = (bounds.min.y - ray.Origin.y) / ray.Direction.y;
	float tymax = (bounds.max.y - ray.Origin.y) / ray.Direction.y;

	if (tymin > tymax) swap(tymin, tymax);

	if ((tMin > tymax) || (tymin > tMax)) return false;
	
	if (tymin > tMin) tMin = tymin;

	if (tymax < tMax) tMax = tymax;

	float tzmin = (bounds.min.z - ray.Origin.z) / ray.Direction.z;
	float tzmax = (bounds.max.z - ray.Origin.z) / ray.Direction.z;

	if (tzmin > tzmax) swap(tzmin, tzmax);
	if ((tMin > tzmax) || (tzmin > tMax))
		return false;

	if (tzmin > tMin) tMin = tzmin;
	if (tzmax < tMax) tMax = tzmax;

	return true;
}
