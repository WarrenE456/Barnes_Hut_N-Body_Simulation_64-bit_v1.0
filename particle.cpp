#include "particle.h"
#include <iostream>
#include <random>

void particle::update(float fps)
{
	position.x += velocity.x / fps;
	position.y += velocity.y / fps;
	position.z += velocity.z / fps;
}
void particle::gravity2D(vec2<float> centerOfMass, double totalMass, double softening, double G)
{
	vec2<float> dist(position.x - centerOfMass.x, position.y - centerOfMass.y);
	float distHyp = std::sqrt(dist.x * dist.x + dist.y * dist.y);

	if (distHyp == 0)
		return;

	if (distHyp <= softening)
	{
		dist.x = dist.x * softening / distHyp;
		dist.y = dist.y * softening / distHyp;
		float force = (G * mass * totalMass) / (softening * softening);
		velocity.x -= force / distHyp * dist.x / mass;
		velocity.y -= force / distHyp * dist.y / mass;
	}
	else
	{
		float force = (G * mass * totalMass) / (distHyp * distHyp);
		velocity.x -= force / distHyp * dist.x / mass;
		velocity.y -= force / distHyp * dist.y / mass;
	}
}
void particle::gravity3D(vec3<float> centerOfMass, double totalMass, double softening, double G)
{
	vec3<float> dist(position.x - centerOfMass.x, position.y - centerOfMass.y, position.z - centerOfMass.z);
	float distHyp = sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);

	if (distHyp == 0)
		return;

	if (distHyp <= softening)
	{
		dist.x = dist.x * softening / distHyp;
		dist.y = dist.y * softening / distHyp;
		dist.z = dist.z * softening / distHyp;

		float force = (G * mass * totalMass) / (softening * softening);
		velocity.x -= force / distHyp * dist.x / mass;
		velocity.y -= force / distHyp * dist.y / mass;
		velocity.z -= force / distHyp * dist.z / mass;
	}
	else
	{
		float force = (G * mass * totalMass) / (distHyp * distHyp);
		velocity.x -= force / distHyp * dist.x / mass;
		velocity.y -= force / distHyp * dist.y / mass;
		velocity.z -= force / distHyp * dist.z / mass;
	}
}