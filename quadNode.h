#pragma once

#include "vectors.h";
#include "particle.h"

class boundary2D
{
public:
	float top;
	float bottom;
	float left;
	float right;

	boundary2D();
	bool contains(vec2<float> position);
};

class quadNode
{
public:
	int depth;
	particle* remember;
	bool subdevided;
	double totalMass;
	vec2<float> centerOfMass;
	quadNode* quadrant[4];
	boundary2D bounds;

	quadNode(int depth);
	void updateMass(particle* part);
	void subdevide(int d);
	void insert(particle* part, unsigned int maxTreeDepth);
	void disassemble();
	void searchForNodeThatSatisfysThreshold(particle* part, float thresholdTheta, double softening, double G);
};