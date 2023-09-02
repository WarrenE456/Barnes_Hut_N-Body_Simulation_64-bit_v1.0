
#include "particle.h"
#include "quadNode.h"
#include "octNode.h"

void THREADCreateRootNodeBoundingBox2D(particle* parts, int segment, boundary2D* bounds, unsigned long quantity, unsigned int multiThreadCount)
{
	for (int i = quantity / multiThreadCount * segment; i < quantity / multiThreadCount * (segment + 1); i++)
	{
		if (parts[i].position.x > bounds->right)
			bounds->right = parts[i].position.x;

		if (parts[i].position.x < bounds->left)
			bounds->left = parts[i].position.x;

		if (parts[i].position.y > bounds->top)
			bounds->top = parts[i].position.y;

		if (parts[i].position.y < bounds->bottom)
			bounds->bottom = parts[i].position.y;
	}
	return;
}

void THREADCreateRootNodeBoundingBox3D(particle* parts, int segment, boundary3D* bounds, unsigned long quantity, unsigned int multiThreadCount)
{
	for (int i = quantity / multiThreadCount * segment; i < quantity / multiThreadCount * (segment + 1); i++)
	{
		if (parts[i].position.x > bounds->right)
			bounds->right = parts[i].position.x;

		if (parts[i].position.x < bounds->left)
			bounds->left = parts[i].position.x;

		if (parts[i].position.y > bounds->top)
			bounds->top = parts[i].position.y;

		if (parts[i].position.y < bounds->bottom)
			bounds->bottom = parts[i].position.y;

		if (parts[i].position.z > bounds->front)
			bounds->front = parts[i].position.z;

		if (parts[i].position.z < bounds->back)
			bounds->back = parts[i].position.z;
	}
	return;
}