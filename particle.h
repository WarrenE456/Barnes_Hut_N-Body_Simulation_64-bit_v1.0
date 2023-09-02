#pragma once

#include "vectors.h"

class particle
{
public:
	vec3<float> position;
	vec3<float> velocity;
	double mass;

	void update(float fps);
	void gravity2D(vec2<float> centerOfMass, double totalMass, double softening, double G);
	void gravity3D(vec3<float> centerOfMass, double totalMass, double softening, double G);
};