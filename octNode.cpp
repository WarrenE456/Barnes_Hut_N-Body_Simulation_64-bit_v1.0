#include "octNode.h"

boundary3D::boundary3D()
	:top(0), bottom(0), left(0), right(0), back(0), front(0) {}

bool boundary3D::contains(vec3<float> position)
{
	return (position.y <= top && position.y >= bottom && position.x <= right && position.x >= left && position.z <= front && position.z >= back);
}

int depth;
particle* remember;
bool subdevided;
double totalMass;
vec3<float> centerOfMass;
octNode* quadrant[8];
boundary3D bounds;

octNode::octNode(int depth)
	:depth(depth), remember(nullptr), subdevided(false), totalMass(0), centerOfMass{ 0,0,0 }, quadrant{ 0,0,0,0,0,0,0,0 } {}


void octNode::updateMass(particle* part)
{

	centerOfMass.x = (totalMass * centerOfMass.x + part->mass * part->position.x) / (totalMass + part->mass);
	centerOfMass.y = (totalMass * centerOfMass.y + part->mass * part->position.y) / (totalMass + part->mass);
	centerOfMass.z = (totalMass * centerOfMass.z + part->mass * part->position.z) / (totalMass + part->mass);

	totalMass += part->mass;
}


void octNode::subdevide(int depth)
{

	for (int i = 0; i < 8; i++)
		quadrant[i] = new octNode(depth);

	quadrant[0]->bounds.top = bounds.top;
	quadrant[0]->bounds.bottom = (bounds.top + bounds.bottom) / 2;
	quadrant[0]->bounds.right = bounds.right;
	quadrant[0]->bounds.left = (bounds.right + bounds.left) / 2;
	quadrant[0]->bounds.front = bounds.front;
	quadrant[0]->bounds.back = (bounds.front + bounds.back) / 2;

	quadrant[4]->bounds.top = bounds.top;
	quadrant[4]->bounds.bottom = (bounds.top + bounds.bottom) / 2;
	quadrant[4]->bounds.right = bounds.right;
	quadrant[4]->bounds.left = (bounds.right + bounds.left) / 2;
	quadrant[4]->bounds.front = (bounds.front + bounds.back) / 2;
	quadrant[4]->bounds.back = bounds.back;

	quadrant[1]->bounds.top = bounds.top;
	quadrant[1]->bounds.bottom = (bounds.top + bounds.bottom) / 2;
	quadrant[1]->bounds.right = (bounds.right + bounds.left) / 2;
	quadrant[1]->bounds.left = bounds.left;
	quadrant[1]->bounds.front = bounds.front;
	quadrant[1]->bounds.back = (bounds.front + bounds.back) / 2;

	quadrant[5]->bounds.top = bounds.top;
	quadrant[5]->bounds.bottom = (bounds.top + bounds.bottom) / 2;
	quadrant[5]->bounds.right = (bounds.right + bounds.left) / 2;
	quadrant[5]->bounds.left = bounds.left;
	quadrant[5]->bounds.front = (bounds.front + bounds.back) / 2;
	quadrant[5]->bounds.back = bounds.back;

	quadrant[2]->bounds.top = (bounds.top + bounds.bottom) / 2;
	quadrant[2]->bounds.bottom = bounds.bottom;
	quadrant[2]->bounds.right = (bounds.right + bounds.left) / 2;
	quadrant[2]->bounds.left = bounds.left;
	quadrant[2]->bounds.front = bounds.front;
	quadrant[2]->bounds.back = (bounds.front + bounds.back) / 2;

	quadrant[6]->bounds.top = (bounds.top + bounds.bottom) / 2;
	quadrant[6]->bounds.bottom = bounds.bottom;
	quadrant[6]->bounds.right = (bounds.right + bounds.left) / 2;
	quadrant[6]->bounds.left = bounds.left;
	quadrant[6]->bounds.front = (bounds.front + bounds.back) / 2;
	quadrant[6]->bounds.back = bounds.back;


	quadrant[3]->bounds.top = (bounds.top + bounds.bottom) / 2;
	quadrant[3]->bounds.bottom = bounds.bottom;
	quadrant[3]->bounds.right = bounds.right;
	quadrant[3]->bounds.left = (bounds.right + bounds.left) / 2;
	quadrant[3]->bounds.front = bounds.front;
	quadrant[3]->bounds.back = (bounds.front + bounds.back) / 2;

	quadrant[7]->bounds.top = (bounds.top + bounds.bottom) / 2;
	quadrant[7]->bounds.bottom = bounds.bottom;
	quadrant[7]->bounds.right = bounds.right;
	quadrant[7]->bounds.left = (bounds.right + bounds.left) / 2;
	quadrant[7]->bounds.front = (bounds.front + bounds.back) / 2;
	quadrant[7]->bounds.back = bounds.back;
}

void octNode::insert(particle* part, unsigned int maxTreeDepth)
{

	if (!bounds.contains(part->position))
		return;

	this->updateMass(part);

	if (!remember)
	{
		remember = part;
		return;
	}

	if (depth == maxTreeDepth)
		return;

	if (!subdevided)
	{

		this->subdevide(depth + 1);

		subdevided = true;

		for (int i = 0; i < 8; i++)
			quadrant[i]->insert(remember, maxTreeDepth);
	}

	for (int i = 0; i < 8; i++)
		quadrant[i]->insert(part, maxTreeDepth);

	return;
}

void octNode::disassemble()
{

	if (!subdevided)
		return;

	for (int i = 0; i < 8; i++)
	{
		quadrant[i]->disassemble();
		delete quadrant[i];
	}
	return;
}

void octNode::searchForNodeThatSatisfysThreshold(particle* part, float thresholdTheta , double softening, double G)
{

	if (totalMass == 0)
		return;

	if (!subdevided || (((bounds.right - bounds.left) * (bounds.front - bounds.back)) / ((part->position.x - centerOfMass.x) * (part->position.x - centerOfMass.x) + (part->position.y - centerOfMass.y) * (part->position.y - centerOfMass.y) * (part->position.x - centerOfMass.x) * (part->position.z - centerOfMass.z)) <= (thresholdTheta * thresholdTheta)))
	{
		part->gravity3D(centerOfMass, totalMass, softening, G);
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			quadrant[i]->searchForNodeThatSatisfysThreshold(part, thresholdTheta, softening, G);
		}
	}
}