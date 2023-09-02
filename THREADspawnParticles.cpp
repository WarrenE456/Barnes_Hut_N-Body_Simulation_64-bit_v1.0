#include "THREADspawnParticles.h"

#include "particle.h"
#include<iostream>
#include <random>
#include "matrix.h"
#include "enum.h"
#include "vectors.h"
#include "configurations.h"
#include "roundUpToNearestInt.h"
 
const float PI = 3.1415926535;

void THREADSpawnParticles2D(particle* parts, int segment, unsigned long* totalSpawned, configuration* config)
{
	float angle;
	float distance;
	std::random_device rd;
	std::uniform_int_distribution<long long> massDistribution(config->minMass * pow(10, config->decimalPercision), config->maxMass * pow(10, config->decimalPercision));

	unsigned int index = 0;
	for (int j = 0; j < config->galaxyCount; j++)
	{
		std::uniform_int_distribution<long> coordinateDistribution(-config->galaxyParam2D[j].radius * pow(10, config->decimalPercision), config->galaxyParam2D[j].radius * pow(10, config->decimalPercision));
		std::uniform_int_distribution<long> radiusDistribution(0, config->galaxyParam2D[j].radius * pow(10, config->decimalPercision));
		std::uniform_int_distribution<long> angleDistribution(0, 2 * PI * pow(10, config->decimalPercision));

		matrix3x3<float> spiralRotationMatrix(
			cos(90), -sin(90), 0,
			sin(90), cos(90), 0,
			0, 0, 1
		);

		unsigned int workPerThread = 0;
		
		if (config->galaxyParam2D[j].quantity % config->multiThreadCount == 0)
		{
			workPerThread = config->galaxyParam2D[j].quantity / config->multiThreadCount;
		}
		else
		{
			int(config->galaxyParam2D[j].quantity / config->multiThreadCount + 1);
		}

		for (int i = workPerThread * segment; i < workPerThread * (segment + 1); i++)
		{
			if (i < config->galaxyParam2D[j].quantity)
			{
				if (config->minMass == config->maxMass)
				{
					parts[i + index].mass = config->minMass;
				}
				else
				{
					parts[i + index].mass = massDistribution(rd) / pow(10, config->decimalPercision);
				}

				if (config->galaxyParam2D[j].shape != spawnOptions2D::circleCentral)
				{
					while (true)
					{
						parts[i + index].position.x = coordinateDistribution(rd) / pow(10, config->decimalPercision);
						parts[i + index].position.y = coordinateDistribution(rd) / pow(10, config->decimalPercision);
						distance = std::sqrt((parts[i + index].position.x * parts[i + index].position.x) + (parts[i + index].position.y * parts[i + index].position.y));

						if (config->galaxyParam2D[j].shape == spawnOptions2D::square)
							break;
						if (distance <= config->galaxyParam2D[j].radius)
							break;
					}
				}
				else
				{
					distance = radiusDistribution(rd) / pow(10, config->decimalPercision);
					angle = angleDistribution(rd) / pow(10, config->decimalPercision);

					parts[i + index].position.x = cos(angle) * distance;
					parts[i + index].position.y = sin(angle) * distance;
				}

				if (config->galaxyParam2D[j].initialVelocity[velocitymodeindex::spiral] != 0)
				{
					if (config->galaxyParam2D[j].dynamicExpansion)
					{
						parts[i + index].velocity.x += (parts[i + index].position.x * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::spiral]) / distance * (distance / config->galaxyParam2D[j].radius);
						parts[i + index].velocity.y += (parts[i + index].position.y * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::spiral]) / distance * (distance / config->galaxyParam2D[j].radius);
					}
					else
					{
						parts[i + index].velocity.x += (parts[i + index].position.x * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::spiral]) / distance;
						parts[i + index].velocity.y += (parts[i + index].position.y * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::spiral]) / distance;
					}
					parts[i + index].velocity = spiralRotationMatrix * parts[i + index].velocity;
				}

				if (config->galaxyParam2D[j].initialVelocity[velocitymodeindex::outward] != 0)
				{
					if (config->galaxyParam2D[j].dynamicExpansion)
					{
						parts[i + index].velocity.x += (parts[i + index].position.x * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::outward]) / distance * (distance / config->galaxyParam2D[j].radius);
						parts[i + index].velocity.y += (parts[i + index].position.y * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::outward]) / distance * (distance / config->galaxyParam2D[j].radius);
					}
					else
					{
						parts[i + index].velocity.x += (parts[i + index].position.x * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::outward]) / distance;
						parts[i + index].velocity.y += (parts[i + index].position.y * config->galaxyParam2D[j].initialVelocity[velocitymodeindex::outward]) / distance;
					}
				}

				parts[i + index].velocity.x += config->galaxyParam2D[j].initialVelocity[velocitymodeindex::directionx];
				parts[i + index].velocity.y += config->galaxyParam2D[j].initialVelocity[velocitymodeindex::directiony];

				parts[i + index].position.x += config->galaxyParam2D[j].centerLocation.x;
				parts[i + index].position.y += config->galaxyParam2D[j].centerLocation.y;

				(*totalSpawned)++;
			}
		}
		index += config->galaxyParam2D[j].quantity;
	}

	return;
}

void THREADSpawnParticles3D(particle* parts, int segment, unsigned long* totalgenerated, configuration* config)
{

	float angle;
	float distance;
	float thickness;
	std::random_device rd;
	std::uniform_int_distribution<long long> massDistribution(config->minMass * pow(10, config->decimalPercision), config->maxMass * pow(10, config->decimalPercision));

	unsigned int index = 0;
	for (int j = 0; j < config->galaxyCount; j++)
	{
		std::uniform_int_distribution<long> angleDistribution(0, 2 * PI * pow(10, config->decimalPercision));
		std::uniform_int_distribution<long> distanceDististribution(0, config->galaxyParam3D[j].radius * pow(10, config->decimalPercision));
		std::uniform_int_distribution<long> coordDistribution(-config->galaxyParam3D[j].radius * pow(10, config->decimalPercision), config->galaxyParam3D[j].radius * pow(10, config->decimalPercision));
		std::uniform_int_distribution<long> thicknessDistribution(config->galaxyParam3D[j].depth / 2 * -pow(10, config->decimalPercision), config->galaxyParam3D[j].depth / 2 * pow(10, config->decimalPercision));
		
		matrix3x3<float> xAxisRotationMatrix(
			1, 0, 0,
			0, cos(config->galaxyParam3D[j].orientation.x), -sin(config->galaxyParam3D[j].orientation.x),
			0, sin(config->galaxyParam3D[j].orientation.x), cos(config->galaxyParam3D[j].orientation.x)
		);

		matrix3x3<float> yAxisRotationMatrix(
			cos(config->galaxyParam3D[j].orientation.y), 0, sin(config->galaxyParam3D[j].orientation.y),
			0, 1, 0,
			-sin(config->galaxyParam3D[j].orientation.y), 0, cos(config->galaxyParam3D[j].orientation.y)
		);

		matrix3x3<float> zAxisRotationMatrix(
			cos(config->galaxyParam3D[j].orientation.z), -sin(config->galaxyParam3D[j].orientation.z), 0,
			sin(config->galaxyParam3D[j].orientation.z), cos(config->galaxyParam3D[j].orientation.z), 0,
			0, 0, 1
		);

		matrix3x3<float> rotationMatrix = zAxisRotationMatrix * yAxisRotationMatrix * xAxisRotationMatrix;

		matrix3x3<float> spiralRotationMatrix(
			cos(90), -sin(90), 0,
			sin(90), cos(90), 0,
			0, 0, 1
		);

		unsigned int workPerThread = 0;

		if (config->galaxyParam3D[j].quantity % config->multiThreadCount == 0)
		{
			workPerThread = config->galaxyParam3D[j].quantity / config->multiThreadCount;
		}
		else
		{
			int(config->galaxyParam3D[j].quantity / config->multiThreadCount + 1);
		}

		for (int i = workPerThread * segment; i < workPerThread * (segment + 1); i++)
		{
			if (i + index < config->quantity)
			{
				if (config->minMass == config->maxMass)
				{
					parts[i + index].mass = config->minMass;
				}
				else
				{
					parts[i + index].mass = massDistribution(rd) / pow(10, config->decimalPercision);
				}

				if (config->galaxyParam3D[j].shape != spawnOptions3D::diskCentral)
				{
					while (true)
					{
						parts[i + index].position.x = coordDistribution(rd) / pow(10, config->decimalPercision);
						parts[i + index].position.y = coordDistribution(rd) / pow(10, config->decimalPercision);
						parts[i + index].position.z = coordDistribution(rd) / pow(10, config->decimalPercision);

						if (config->galaxyParam3D[j].shape == spawnOptions3D::cuboid)
							break;

						if (std::sqrt((parts[i + index].position.x * parts[i + index].position.x) + (parts[i + index].position.y * parts[i + index].position.y) + (parts[i + index].position.z * parts[i + index].position.z)) <= config->galaxyParam3D[j].radius)
							break;
					}

					parts[i + index].position.z = parts[i + index].position.z * (config->galaxyParam3D[j].depth / config->galaxyParam3D[j].radius);

					distance = std::sqrt((parts[i + index].position.x * parts[i + index].position.x) + (parts[i + index].position.y * parts[i + index].position.y));
				}
				else
				{
					distance = distanceDististribution(rd) / pow(10, config->decimalPercision);
					thickness = thicknessDistribution(rd) / pow(10, config->decimalPercision);
					angle = angleDistribution(rd) / pow(10, config->decimalPercision);

					parts[i + index].position.x = distance * cos(angle) + config->galaxyParam3D[j].centerLocation.x;
					parts[i + index].position.y = distance * sin(angle) + config->galaxyParam3D[j].centerLocation.y;
					parts[i + index].position.z = thickness * sqrt(config->galaxyParam3D[j].radius * config->galaxyParam3D[j].radius - distance * distance) / config->galaxyParam3D[j].radius;
				}

				if (config->galaxyParam3D[j].initialVelocity[spiral] != 0)
				{
					parts[i + index].velocity.x += config->galaxyParam3D[j].initialVelocity[spiral] * parts[i + index].position.x / distance;
					parts[i + index].velocity.y += config->galaxyParam3D[j].initialVelocity[spiral] * parts[i + index].position.y / distance;
					parts[i + index].velocity = spiralRotationMatrix * parts[i + index].velocity;
					if (config->galaxyParam3D[j].dynamicSpiral)
					{
						parts[i + index].velocity.x = parts[i + index].velocity.x * (distance / config->galaxyParam3D[j].radius);
						parts[i + index].velocity.y = parts[i + index].velocity.y * (distance / config->galaxyParam3D[j].radius);
					}
				}
				if (config->galaxyParam3D[j].initialVelocity[outward] != 0)
				{
					float dist = std::sqrt((parts[i + index].position.x * parts[i + index].position.x) + (parts[i + index].position.y * parts[i + index].position.y) + (parts[i + index].position.z * parts[i + index].position.z));

					if (config->galaxyParam3D[j].dynamicExpansion)
					{
						parts[i + index].velocity.x += config->galaxyParam3D[j].initialVelocity[outward] * parts[i + index].position.x / dist * (dist / config->galaxyParam3D[j].radius);
						parts[i + index].velocity.y += config->galaxyParam3D[j].initialVelocity[outward] * parts[i + index].position.y / dist * (dist / config->galaxyParam3D[j].radius);
						parts[i + index].velocity.z += config->galaxyParam3D[j].initialVelocity[outward] * parts[i + index].position.z / dist * (dist / config->galaxyParam3D[j].radius);
					}
					else
					{
						parts[i + index].velocity.x += config->galaxyParam3D[j].initialVelocity[outward] * parts[i + index].position.x / dist;
						parts[i + index].velocity.y += config->galaxyParam3D[j].initialVelocity[outward] * parts[i + index].position.y / dist;
						parts[i + index].velocity.z += config->galaxyParam3D[j].initialVelocity[outward] * parts[i + index].position.z / dist;
					}
				}

				parts[i + index].position = rotationMatrix * parts[i + index].position;
				parts[i + index].velocity = rotationMatrix * parts[i + index].velocity;

				parts[i + index].position.x += config->galaxyParam3D[j].centerLocation.x;
				parts[i + index].position.y += config->galaxyParam3D[j].centerLocation.y;
				parts[i + index].position.z += config->galaxyParam3D[j].centerLocation.z;

				parts[i + index].velocity.x += config->galaxyParam3D[j].initialVelocity[directionx];
				parts[i + index].velocity.y += config->galaxyParam3D[j].initialVelocity[directiony];
				parts[i + index].velocity.z += config->galaxyParam3D[j].initialVelocity[directionz];

				(*totalgenerated)++;
			}
		}
		index += config->galaxyParam3D[j].quantity;
	}

	return;
}