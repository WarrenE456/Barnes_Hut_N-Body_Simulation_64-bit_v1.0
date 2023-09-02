#include "quadNode.h"

//boundary2D
boundary2D::boundary2D()
	:top(0), bottom(0), left(0), right(0) {}

bool boundary2D::contains(vec2<float> position)
{
	return (position.y <= top && position.y >= bottom && position.x <= right && position.x >= left);
}

//quadNode
quadNode::quadNode(int depth)
	:depth(depth), remember(nullptr), subdevided(false), totalMass(0), centerOfMass{ 0,0 }, quadrant{ nullptr, nullptr, nullptr, nullptr }
{}

void quadNode::updateMass(particle* part)
{

	centerOfMass.x = (totalMass * centerOfMass.x + part->mass * part->position.x) / (totalMass + part->mass);
	centerOfMass.y = (totalMass * centerOfMass.y + part->mass * part->position.y) / (totalMass + part->mass);

	totalMass += part->mass;
}

void quadNode::subdevide(int d)
{

	for (int i = 0; i < 4; i++)
		quadrant[i] = new quadNode(d);

	quadrant[0]->bounds.top = bounds.top;
	quadrant[0]->bounds.bottom = (bounds.top + bounds.bottom) / 2;
	quadrant[0]->bounds.right = bounds.right;
	quadrant[0]->bounds.left = (bounds.right + bounds.left) / 2;

	quadrant[1]->bounds.top = bounds.top;
	quadrant[1]->bounds.bottom = (bounds.top + bounds.bottom) / 2;
	quadrant[1]->bounds.right = (bounds.right + bounds.left) / 2;
	quadrant[1]->bounds.left = bounds.left;

	quadrant[2]->bounds.top = (bounds.top + bounds.bottom) / 2;
	quadrant[2]->bounds.bottom = bounds.bottom;
	quadrant[2]->bounds.right = (bounds.right + bounds.left) / 2;
	quadrant[2]->bounds.left = bounds.left;

	quadrant[3]->bounds.top = (bounds.top + bounds.bottom) / 2;
	quadrant[3]->bounds.bottom = bounds.bottom;
	quadrant[3]->bounds.right = bounds.right;
	quadrant[3]->bounds.left = (bounds.right + bounds.left) / 2;
}

void quadNode::insert(particle* part, unsigned int maxTreeDepth)
{

	if (!bounds.contains(part->position))
		return;

	updateMass(part);

	if (!remember)
	{

		remember = part;
		return;
	}

	if (depth == maxTreeDepth)
		return;

	if (!subdevided)
	{

		subdevide(depth + 1);

		subdevided = true;

		for (int i = 0; i < 4; i++)
			quadrant[i]->insert(remember, maxTreeDepth);
	}

	for (int i = 0; i < 4; i++)
		quadrant[i]->insert(part, maxTreeDepth);

	return;
}

void quadNode::disassemble()
{

	if (!subdevided)
		return;

	for (int i = 0; i < 4; i++)
	{
		quadrant[i]->disassemble();
		delete quadrant[i];
	}
	return;
}


void quadNode::searchForNodeThatSatisfysThreshold(particle* part, float thresholdTheta, double softening, double G)
{

	if (totalMass == 0)
		return;

	if (!subdevided || ((bounds.right - bounds.left) * (bounds.right - bounds.left)) / ((part->position.x - centerOfMass.x) * (part->position.x - centerOfMass.x) + (part->position.y - centerOfMass.y) * (part->position.y - centerOfMass.y)) <= (thresholdTheta * thresholdTheta))
	{
		part->gravity2D(centerOfMass, totalMass, softening, G);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			quadrant[i]->searchForNodeThatSatisfysThreshold(part, thresholdTheta, softening, G);
		}
	}
}