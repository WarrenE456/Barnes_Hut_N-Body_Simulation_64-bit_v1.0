#pragma once
#include <string>
#include "enum.h"
#include "vectors.h"
#include <iostream>

struct galaxyParameters2D
{
	unsigned long quantity;
	unsigned char shape; 
	float radius; 
	float initialVelocity[4];
	bool dynamicSpiral; 
	bool dynamicExpansion;
	vec2<float> centerLocation;

	galaxyParameters2D() : quantity(0), shape(0), radius(0), dynamicSpiral(false), dynamicExpansion(false), initialVelocity{0, 0, 0, 0} {}
};

struct galaxyParameters3D
{
	unsigned long quantity;
	unsigned char shape; 
	float radius;
	float depth;
	bool dynamicSpiral; 
	bool dynamicExpansion;
	float initialVelocity[5];
	vec3<float> centerLocation;
	vec3<float> orientation;

	galaxyParameters3D() : quantity(0), shape(0), radius(0), depth(0), dynamicSpiral(false), dynamicExpansion(false), initialVelocity{ 0, 0, 0, 0, 0 } {}
};

struct configuration
{
	int width; 
	int height;
	float thresholdTheta;
	unsigned int maxTreeDepth;
	float softening; 
	double G;
	unsigned int multiThreadCount; 
	double minMass; 
	double maxMass;
	float particleColor[4]; 
	unsigned long quantity; 
	bool threeDimensions;
	unsigned int galaxyCount;
	galaxyParameters2D* galaxyParam2D;
	galaxyParameters3D* galaxyParam3D;
	float defaultZoom;
	float orbitSensitivity;
	unsigned int decimalPercision;
};