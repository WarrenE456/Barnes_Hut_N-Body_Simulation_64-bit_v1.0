#include "THREADupdate.h"

#include "particle.h"
#include "configurations.h"

void THREADUpdate(particle* parts, float* vertexData, int segment, float fps, configuration* config)
{
	for (int i = config->quantity / config->multiThreadCount * segment; i < config->quantity / config->multiThreadCount * (segment + 1); i++)
	{
		parts[i].update(fps);

		if (!config->threeDimensions)
		{
			vertexData[i * 2] = parts[i].position.x / config->width;
			vertexData[i * 2 + 1] = parts[i].position.y / config->height;
		}
		else
		{
			vertexData[i * 3] = parts[i].position.x;
			vertexData[i * 3 + 1] = parts[i].position.y;
			vertexData[i * 3 + 2] = parts[i].position.z;
		}
	}

	return;
}
