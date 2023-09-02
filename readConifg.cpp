#include <map>
#include <fstream>

#include "configurations.h"

void readConifg(configuration& config, std::string filepath)
{
	std::string line;

#define LINETOINT std::atoi(line.c_str())
#define LINETOFLOAT std::atof(line.c_str())

	std::string mode;
	int i = -1;
	int j = -1;
	
	std::ifstream file;
	file.open(filepath);

	if (file.fail())
		return;

	config.quantity = 0;

	while (getline(file, line))
	{
		if (line[0] == '#')
		{
			line.erase(0, 1);
			mode = line;
		}
		else if (line[1] == '@')
		{
			line.erase(0, 2);
			j = std::atoi(line.c_str());
		}
		else if (line[0] == '@')
		{
			line.erase(0, 1);
			i = std::atoi(line.c_str());
		}
		else
		{
			if (mode.compare("width") == 0)
			{
				config.width = LINETOINT;
			}
			else if (mode.compare("height") == 0)
			{
				config.height = LINETOINT;
			}
			else if (mode.compare("thresholdTheta") == 0)
			{
				config.thresholdTheta = LINETOFLOAT;
			}
			else if (mode.compare("maxTreeDepth") == 0)
			{
				config.maxTreeDepth = LINETOINT;
			}
			else if (mode.compare("softening") == 0)
			{
				config.softening = LINETOFLOAT;
			}
			else if (mode.compare("G") == 0)
			{
				config.G = LINETOFLOAT;
			}
			else if (mode.compare("multiThreadCount") == 0)
			{
				config.multiThreadCount = LINETOINT;
			}
			else if (mode.compare("minMass") == 0)
			{
				config.minMass = LINETOFLOAT;
			}
			else if (mode.compare("maxMass") == 0)
			{
				config.maxMass = LINETOFLOAT;
			}
			else if (mode.compare("particleColor") == 0)
			{
				if (i >= 0)
					config.particleColor[i] = LINETOFLOAT;
			}
			else if (mode.compare("threeDimensions") == 0)
			{
				if (line[0] == 'T')
				{
					config.threeDimensions = true;
				}
				else
				{
					config.threeDimensions = false;
				}
			}
			else if (mode.compare("galaxyCount") == 0)
			{
				config.galaxyCount = LINETOINT;

				if (config.threeDimensions)
				{
					config.galaxyParam3D = new galaxyParameters3D[config.galaxyCount];
				}
				else
				{
					config.galaxyParam2D = new galaxyParameters2D[config.galaxyCount];
				}
			}
			else if (mode.compare("defaultZoom") == 0)
			{
				config.defaultZoom = LINETOFLOAT;
			}
			else if (mode.compare("orbitSensitivity") == 0)
			{
				config.orbitSensitivity = LINETOFLOAT;
			}
			else if (mode.compare("decimalPercision") == 0)
			{
				config.decimalPercision = LINETOINT;
			}
			else if (mode.compare("quantity") == 0)
			{
				config.quantity += LINETOINT;

				if (config.threeDimensions)
				{
					if (i >= 0)
						config.galaxyParam3D[i].quantity = LINETOINT;
				}
				else
				{
					if (i >= 0)
						config.galaxyParam2D[i].quantity = LINETOINT;
				}
			}
			else if (mode.compare("shape") == 0)
			{
				if (config.threeDimensions)
				{
					if (i >= 0)
						config.galaxyParam3D[i].shape = LINETOINT;
				}
				else
				{
					if (i >= 0)
						config.galaxyParam2D[i].shape = LINETOINT;
				}
			}
			else if (mode.compare("radius") == 0)
			{
				if (config.threeDimensions)
				{
					if (i >= 0)
						config.galaxyParam3D[i].radius = LINETOFLOAT;
				}
				else
				{
					if (i >= 0)
						config.galaxyParam2D[i].radius = LINETOFLOAT;
				}
			}
			else if (mode.compare("dynamicSpiral") == 0)
			{
				if (!config.threeDimensions)
				{
					if (line[0] == 'T')
					{
						config.galaxyParam2D[i].dynamicSpiral = true;
					}
					else
					{
						config.galaxyParam2D[i].dynamicSpiral = false;
					}
				}
				if (config.threeDimensions)
				{
					if (line[0] == 'T')
					{
						config.galaxyParam3D[i].dynamicSpiral = true;
					}
					else
					{
						config.galaxyParam3D[i].dynamicSpiral = false;
					}
				}
			}
			else if (mode.compare("dynamicExpansion") == 0)
			{
				if (!config.threeDimensions)
				{
					if (line[0] == 'T')
					{
						config.galaxyParam2D[i].dynamicExpansion = true;
					}
					else
					{
						config.galaxyParam2D[i].dynamicExpansion = false;
					}
				}
				if (config.threeDimensions)
				{
					if (line[0] == 'T')
					{
						config.galaxyParam3D[i].dynamicExpansion = true;
					}
					else
					{
						config.galaxyParam3D[i].dynamicExpansion = false;
					}
				}
			}
			else if (mode.compare("initialVelocity") == 0)
			{	
				if (config.threeDimensions)
				{
					if (i >= 0 && j >= 0)
						config.galaxyParam3D[i].initialVelocity[j] = LINETOFLOAT;
				}
				else
				{
					if (i >= 0 && j >= 0)
						config.galaxyParam2D[i].initialVelocity[j] = LINETOFLOAT;
				}
			}
			else if (mode.compare("centerLocation") == 0)
			{
				if (config.threeDimensions)
				{
					if (i >= 0 && j >= 0)
						config.galaxyParam3D[i].centerLocation[j] = LINETOFLOAT;
				}
				else
				{
					if (i >= 0 && j >= 0)
						config.galaxyParam2D[i].centerLocation[j] = LINETOFLOAT;
				}

			}
			else if (mode.compare("depth") == 0)
			{
				if (i >= 0)
					config.galaxyParam3D[i].depth = LINETOFLOAT;
			}
			else if (mode.compare("orientation") == 0)
			{
				config.galaxyParam3D[i].orientation[j] = LINETOFLOAT;
			}
		}
	}

	file.close();
	return;
}