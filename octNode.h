#pragma once

#include "particle.h"
#include "vectors.h"

class boundary3D
{
public:
	float top;
	float bottom;
	float left;
	float right;
	float back;
	float front;

	boundary3D();
	bool contains(vec3<float> position);
};

class octNode
{
public:
	int depth;
	particle* remember;
	bool subdevided;
	double totalMass;
	vec3<float> centerOfMass;
	octNode* quadrant[8];
	boundary3D bounds;

	octNode(int depth);
	void updateMass(particle* part);
	void subdevide(int depth);
	void insert(particle* part, unsigned int maxTreeDepth);
	void disassemble();
	void searchForNodeThatSatisfysThreshold(particle* part, float thresholdTheta, double softening, double G);
};