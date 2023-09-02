#include <string>
#include <iostream>
#include <fstream>

#include "configurations.h"

std::string line;

#define GETLINE std::getline(std::cin, line)
#define LINETOINT std::atoi(line.c_str())
#define LINETOFLOAT std::atof(line.c_str())

#define CONFIRM(code)\
while(true)\
{\
	code\
	std::cout << "CONFIRM YOUR INPUT: (Y/N);"\
	GETLINE;\
	if (line[0] == 'Y' || line[0] == 'y')\
	{\
		break;\
	}\
}\

void setConfig(configuration& config)
{
	std::string filePath;
	std::string fileName;
	std::ofstream file;

	while (true)
	{
		std::cout << "\nEnter filepath for parameters to be saved: (e.g. C:\\example)\n";
		std::getline(std::cin, filePath);

		std::cout << "\nEnter file name: \n";
		std::getline(std::cin, fileName);
		
		file.open(filePath + "\\" + fileName + ".txt");

		if (!file.fail())
		{
			break;
		}
		std::cout << "\nPlease input valid filepath and file name.\n";
	}
	file.close();
	file.open(filePath + "\\" + fileName + ".txt");

	std::cout << "\nVIDEO SETTINGS:\n"; 

	while (true)
	{
		std::cout << "\nEnter window height:\n";
		GETLINE;

		if (LINETOINT > 0)
		{
			config.height = LINETOINT;
			file << "\n#height\n" << std::to_string(config.height);
			break;
		}
		std::cout << "\nInvalid input! (number may not be 0 or less)\n";
	}

	while (true)
	{
		std::cout << "\nEnter window width:\n";
		GETLINE;

		if (LINETOINT > 0)
		{
			config.width = LINETOINT;
			file << "\n#width\n" << config.width;
			break;
		}
		std::cout << "\nInvalid input! (number may not be 0 or less)\n";
	}
	
	std::cout << "\nSIMULATION PARAMETERS:\n";

	while (true)
	{
		std::cout << "\nWould you like simulation to be 2D (2) or 3D (3): (2/3)\n";
		GETLINE;

		if (LINETOINT == 3)
		{
			config.threeDimensions = true;
			file << "\n#threeDimensions\nT";
			break;
		}
		if (LINETOINT == 2)
		{
			config.threeDimensions = false;
			file << "\n#threeDimensions\nF";
			break;
		}

		std::cout << "\nInvalid input! (number must be either 2, or 3)\n";
	}

	while (true)
	{
		std::cout << "\nEnter perfered decimal percision: (/about for more information)\n";
		GETLINE;

		if (line.compare("/about") == 0)
		{
			std::cout << "\nABOUT: Decimal percision is the number of decimal places used in things like random number generation. (recomended 5-10)\n";
		}
		else
		{
			if (LINETOINT > 0)
			{
				config.decimalPercision = LINETOINT;
				file << "\n#decimalPercision\n" << std::to_string(config.decimalPercision);
				break;
			}
			std::cout << "\nInvalid input! (number may not be 0 or less)\n";
		}
	}

	while (true)
	{
		std::cout << "\nInput theta threshold: (/about for more information)\n";
		GETLINE;

		if (line.compare("/about") == 0)
		{
			std::cout << "\nABOUT: In the Barnes hut algorithm the ratio between the side length of the node and the distance between the node and the particle must be below some threshold, called\n the theta value.Lower theta means more accuracy, higher theta\n means more performance. (recomended theta value 0.3-1.0)\n";
		}
		else
		{
			config.thresholdTheta = LINETOFLOAT;
			file << "\n#thresholdTheta\n" << std::to_string(config.thresholdTheta);
			break;
		}
	}

	while (true)
	{
		std::cout << "\nInput maximum oct/quadtree depth: (/about for more information)\n";
		GETLINE;

		if (line.compare("/about") == 0)
		{
			std::cout << "\nABOUT: The barnes hut algorithm uses tree-type data structures to store regions of particles.\nA limit on tree depth prevents stackoverflow from excesive recursion. (recommended 8-12)\n";
		}
		else
		{
			if (LINETOINT > 0)
			{
				config.maxTreeDepth = LINETOINT;
				file << "\n#maxTreeDepth\n" << std::to_string(config.maxTreeDepth);
				break;
			}
			std::cout << "\nEnter vailid input. (can NOT be 0 or less)\n";
		}
	}

	while (true)
	{
		std::cout << "\nInput softening treshold (pixles): (/about for more information)\n";
		GETLINE;

		if (line.compare("/about") == 0)
		{
			std::cout << "\nABOUT: The force of gravity is inversely porportional to the distance between two bodies.\nTo prevent the force magtitude from going to infinity in a collisionless simulation a minimum distance between gravitati-\nional bodies is enforced, called the softening.\n(recomended 2.0-5.0)\n";
		}
		else
		{
			config.softening = abs(LINETOFLOAT);
			file << "\n#softening\n" << std::to_string(config.softening);
			break;
		}
	}

	std::cout << "\nInput gravity strength multiplier: (this number CAN be negetive) (Disclaimer: 0 removes gravity entirely)\n";
	GETLINE;
	config.G = pow(6.6743, -11) * LINETOFLOAT;
	file << "\n#G\n" << config.G;

	while (true)
	{
		std::cout << "\nInput addition thread count: (can NOT be less than 0) (/about for more information)\n";
		GETLINE;
		
		if (line.compare("/about") == 0)
		{
			std::cout << "\nABOUT: The number of additional threads used for multithreaded tasks.\n(check number of cores in CPU and multiply by 2 for best performance)\n";
		}
		else
		{
			if (LINETOINT > 0)
			{
				config.multiThreadCount = LINETOINT;
				file << "\n#multiThreadCount\n" << std::to_string(config.multiThreadCount);
				break;
			}
			std::cout << "\nInvalid input! (can NOT be less than 0)\n";
		}
	}
	
	std::cout << "\nSPAWNPARAMETERS:\n"; 

	std::cout << "\nInput minimum mass for particles: (particles' mass will be random chosen between the min and max) (if min and max are equal, then all particles will be that mass)\n";
	GETLINE;
	config.minMass = LINETOFLOAT;
	file << "\n#minMass\n" << std::to_string(config.minMass);

	while (true)
	{
		std::cout << "\nInput maximum mass for particles: (particles' mass will be random chosen between the min and max) (if min and max are equal, then all particles will be that mass)\n";
		GETLINE;
		config.maxMass = LINETOFLOAT;
		if (config.maxMass >= config.minMass)
		{
			file << "\n#maxMass\n" << std::to_string(config.maxMass);
			break;
		}
		std::cout << "\nEnter valid input: (maximum must be greater than or equal to minimum)\n";
	}

	std::cout << "\nInput particle color red value: (0-255) (negative values will become 0);\n";
	GETLINE;

	config.particleColor[0] = LINETOFLOAT;
	if (config.particleColor[0] < 0)
		config.particleColor[0] = 0;
	else if (config.particleColor[0] > 255)
		config.particleColor[0] = 255;

	file << "\n@0" << "\n#particleColor\n" << config.particleColor[0] << "\n";


	std::cout << "\nInput particle color green value: (0-255) (negative values will become 0)\n";
	GETLINE;

	config.particleColor[1] = LINETOFLOAT;
	if (config.particleColor[1] < 0)
		config.particleColor[1] = 0;
	else if (config.particleColor[1] > 255)
		config.particleColor[1] = 255;

	file << "@1\n" << config.particleColor[1] << "\n";

	std::cout << "\nInput particle color blue value: (0-255) (negative values will become 0)\n";
	GETLINE;

	config.particleColor[2] = LINETOFLOAT;
	if (config.particleColor[2] < 0)
		config.particleColor[2] = 0;
	else if (config.particleColor[2] > 255)
		config.particleColor[2] = 255;

	file << "@2\n" << config.particleColor[2] << "\n";

	std::cout << "\nInput particle color alpha value: (0-100%) (0% = transparent, 100% = opaque) (negative values will become 0)\n";
	GETLINE;

	config.particleColor[3] = LINETOFLOAT;
	if (config.particleColor[3] < 0)
		config.particleColor[3] = 0;
	else if (config.particleColor[3] > 100)
		config.particleColor[3] = 100;

	file << "@3\n" << config.particleColor[3];

	while (true)
	{
		std::cout << "\nInput galaxy count:\n";
		GETLINE;

		if (LINETOINT > 0)
		{
			config.galaxyCount = LINETOINT;
			file << "\n#galaxyCount\n" << std::to_string(config.galaxyCount);
			break;
		}
		std::cout << "\nInvalid input! (number can NOT be 0 or less)\n";
	}

	config.quantity = 0;
	if (config.threeDimensions)
	{
		config.galaxyParam3D = new galaxyParameters3D[config.galaxyCount];
		config.galaxyParam2D = nullptr;

		file << "\n#galaxyParam3D";

		for (int i = 0; i < config.galaxyCount; i++)
		{
			file << "\n@" << i;
			std::cout << "\nGalaxy: (" << i + 1 << "/" << config.galaxyCount << ")\n";

			while (true)
			{
				std::cout << "\nInput number of particles in galaxy:\n";
				GETLINE;

				if (LINETOINT > 0)
				{
					config.galaxyParam3D[i].quantity = LINETOINT;
					config.quantity += config.galaxyParam3D[i].quantity;
					if (i == config.galaxyCount - 1)
					{
						if (config.quantity % config.multiThreadCount != 0)
						{
							std::cout << "\nThe total quantity of particles was not devisible by the additional thread count.\nTo utalize multithreading most effectivly " << config.quantity % config.multiThreadCount << " was added to the quantity of particles in the this galaxy.\n";
							config.galaxyParam3D[i].quantity += config.quantity % config.multiThreadCount;
						}
					}
					file << "\n#quantity\n" << std::to_string(config.galaxyParam3D[i].quantity);
					break;
				}

				std::cout << "\nEnter valid input: (number can NOT be 0 or less)\n";
			}

			while (true)
			{
				std::cout << "\nInput galaxy shpape. Cuboid (1), disk with central distribution (2), or disk with even distribution (3): (1/2/3)\n";
				GETLINE;

				if (1 <= LINETOINT && LINETOINT <= 3)
				{
					config.galaxyParam3D[i].shape = LINETOINT - 1;
					file << "\n#shape\n" << (int)config.galaxyParam3D[i].shape;
					break;
				}

				std::cout << "\nInvalid input! (number must be between (or equal to) 1 and 3)\n";
			}

			//spiral, outward, x , y , z 
			std::cout << "\nInput inital tangent velocity (pixles/second) of particles: (how fast should the galaxy spin) (number CAN be negative to change direction of rotation)\n";
			GETLINE;
			config.galaxyParam3D[i].initialVelocity[0] = LINETOFLOAT;
			file <<"\n@@0\n" << "#initialVelocity\n" << std::to_string(config.galaxyParam3D[i].initialVelocity[0]);

			std::cout << "\nInput inital outward velocity (pixles/second) of particles: (number CAN be negative to make velocity inward)\n";
			GETLINE;
			config.galaxyParam3D[i].initialVelocity[1] = LINETOFLOAT;
			file << "\n@@1\n" << std::to_string(config.galaxyParam3D[i].initialVelocity[1]);

			std::cout << "\nInput inital X (side to side) velocity (pixles/second) of particles: (number CAN be negative to change direction)\n";
			GETLINE;
			config.galaxyParam3D[i].initialVelocity[2] = LINETOFLOAT;
			file << "\n@@2\n" << std::to_string(config.galaxyParam3D[i].initialVelocity[2]);

			std::cout << "\nInput inital Y (up and down) velocity (pixles/second) of particles: (number CAN be negative to change direction)\n";
			GETLINE;
			config.galaxyParam3D[i].initialVelocity[3] = LINETOFLOAT;
			file << "\n@@3\n" << std::to_string(config.galaxyParam3D[i].initialVelocity[3]);

			std::cout << "\nInput inital Z (back and forth) velocity (pixles/second) of particles: (number CAN be negative to change direction)\n";
			GETLINE;
			config.galaxyParam3D[i].initialVelocity[4] = LINETOFLOAT;
			file << "\n@@4\n" << std::to_string(config.galaxyParam3D[i].initialVelocity[4]);

			while (true)
			{
				if (config.galaxyParam3D[i].shape == 0)
					std::cout << "\nInput side lengths of galaxy: (number can NOT be 0 or less)\n";
				else
					std::cout << "\nInput spawn radius of galaxy: (number can NOT be 0 or less)\n";

				GETLINE;
				config.galaxyParam3D[i].radius = LINETOFLOAT;

				if (config.galaxyParam3D[i].radius > 0)
				{
					file << "\n#radius\n" << std::to_string(config.galaxyParam3D[i].radius);
					break;
				}
				std::cout << "\nEnter valid input:  (number can NOT be 0 or less)\n";
			}

			while (true)
			{
				std::cout << "\nInput depth of galaxy: (number can NOT be 0 or less)\n";

				GETLINE;
				config.galaxyParam3D[i].depth = LINETOFLOAT;

				if (config.galaxyParam3D[i].depth > 0)
				{
					file << "\n#depth\n" << std::to_string(config.galaxyParam3D[i].depth);
					break;
				}
				std::cout << "\nEnter valid input:  (number can NOT be 0 or less)\n";
			}

			std::cout << "\nInput galaxy's intital orientation (rad) about the X-axis:\n";
			GETLINE;
			config.galaxyParam3D[i].orientation.x = LINETOFLOAT;
			file << "\n@@0" << "\n#orientation\n" << std::to_string(config.galaxyParam3D[i].orientation.x);

			std::cout << "\nInput galaxy's intital orientation (rad) about the Y-axis:\n";
			GETLINE;
			config.galaxyParam3D[i].orientation.y = LINETOFLOAT;
			file << "\n@@1\n" << std::to_string(config.galaxyParam3D[i].orientation.y);

			std::cout << "\nInput galaxy's intital orientation (rad) about the Z-axis:\n";
			GETLINE;
			config.galaxyParam3D[i].orientation.z = LINETOFLOAT;
			file << "\n@@2\n" << std::to_string(config.galaxyParam3D[i].orientation.z);

			std::cout << "\nInput galaxy's intital center X location:\n";
			GETLINE;
			config.galaxyParam3D[i].centerLocation.x = LINETOFLOAT;
			file << "\n@@0\n" << "#centerLocation\n" << std::to_string(config.galaxyParam3D[i].centerLocation.x);

			std::cout << "\nInput galaxy's intital center Y location:\n";
			GETLINE;
			config.galaxyParam3D[i].centerLocation.y = LINETOFLOAT;
			file << "\n@@1\n" << std::to_string(config.galaxyParam3D[i].centerLocation.y);

			std::cout << "\nInput galaxy's intital center Z location:\n";
			GETLINE;
			config.galaxyParam3D[i].centerLocation.z = LINETOFLOAT;
			file << "\n@@2\n" << std::to_string(config.galaxyParam3D[i].centerLocation.z);
			
			while (true)
			{
				std::cout << "\nEnable dynamic spiral (Y), or not (N): (Y/N) (/about for more information)\n";
				GETLINE;

				if (line.compare("/about") == 0)
				{
					std::cout << "\nABOUT: Dynamic spiral slows particle's inital tangent velocity according to the particle's distance from the center of the galaxy.\n This prevents a wierd hole in the middle of the galaxy from forming.\n";
				}
				else
				{
					if (line[0] == 'Y' || line[0] == 'y')
					{
						config.galaxyParam3D[i].dynamicSpiral = true;
						file << "\n#dynamicSpiral\nT";
						break;
					}
					if (line[0] == 'N' || line[0] == 'n')
					{
						config.galaxyParam3D[i].dynamicSpiral = false;
						file << "\n#dynamicSpiral\nF";
						break;
					}

					std::cout << "\nEnter valid input: (must be \'Y\', or \'N\')\n";
				}
			}

			while (true)
			{
				std::cout << "\nEnable dynamic expansion (Y), or not (N): (Y/N) (/about for more information)\n";
				GETLINE;

				if (line.compare("/about") == 0)
				{
					std::cout << "\nABOUT: Dynamic expansion slows particle's inital outward/inward velocity according to the particle's distance from the center of the galaxy.\n This prevents a wierd hole in the middle of the galaxy from forming.\n";
				}
				else
				{
					if (line[0] == 'Y' || line[0] == 'y')
					{
						config.galaxyParam3D[i].dynamicExpansion = true;
						file << "\n#dynamicExpansion\nT";
						break;
					}
					if (line[0] == 'N' || line[0] == 'n')
					{
						config.galaxyParam3D[i].dynamicExpansion = false;
						file << "\n#dynamicExpansion\nF";
						break;
					}

					std::cout << "\nEnter valid input: (must be \'Y\', or \'N\')";
				}
			}
		}

	}
	else
	{
		config.galaxyParam2D = new galaxyParameters2D[config.galaxyCount];
		config.galaxyParam3D = nullptr;

		file << "\n#galaxyParam2D\n";

		for (int i = 0; i < config.galaxyCount; i++)
		{
			file << "@" << i;
			std::cout << "Galaxy: (" << i << "/" << config.galaxyCount << ")\n";

			while (true)
			{
				std::cout << "\nInput number of particles in galaxy:\n";
				GETLINE;

				if (LINETOINT > 0)
				{
					config.galaxyParam2D[i].quantity = LINETOINT;
					config.quantity += config.galaxyParam2D[i].quantity;
					file << "\n#quantity\n" << config.galaxyParam2D[i].quantity;
					break;
				}

				std::cout << "\nEnter valid input: (number can NOT be 0 or less)\n";
			}

			while (true)
			{
				std::cout << "\nInput galaxy shpape. Rectangular (1), circle with central distribution (2), or circle with even distribution (3): (1/2/3)\n";
				GETLINE;

				if (1 <= LINETOINT && LINETOINT <= 3)
				{
					config.galaxyParam2D[i].shape = LINETOINT - 1;
					file << "\n#shape\n" << (int)config.galaxyParam2D[i].shape;
					break;
				}

				std::cout << "\nInvalid input! (number must be between (or equal to) 1 and 3)\n";
			}

			//spiral, outward, x , y
			std::cout << "\nInput inital tangent velocity (pixles/second) of particles: (how fast should the galaxy spin) (number CAN be negative to change direction of rotation)\n";
			GETLINE;
			config.galaxyParam2D[i].initialVelocity[0] = LINETOFLOAT;
			file << "\n@@0" << "\n#initialVelocity\n" << config.galaxyParam2D[i].initialVelocity[0];

			std::cout << "\nInput inital outward velocity (pixles/second) of particles: (number CAN be negative to make velocity inward)\n";
			GETLINE;
			config.galaxyParam2D[i].initialVelocity[1] = LINETOFLOAT;
			file << "\n@@1\n" << config.galaxyParam2D[i].initialVelocity[1];

			std::cout << "\nInput inital X (side to side) velocity (pixles/second) of particles: (number CAN be negative to change direction)\n";
			GETLINE;
			config.galaxyParam2D[i].initialVelocity[2] = LINETOFLOAT;
			file << "\n@@2\n" << config.galaxyParam2D[i].initialVelocity[2];

			std::cout << "\nInput inital Y (up and down) velocity (pixles/second) of particles: (number CAN be negative to change direction)\n";
			GETLINE;
			config.galaxyParam2D[i].initialVelocity[3] = LINETOFLOAT;
			file << "\n@@3\n" << config.galaxyParam2D[i].initialVelocity[3];

			while (true)
			{
				if (config.galaxyParam2D[i].shape == 0)
					std::cout << "\nInput side lengths of galaxy: (number can NOT be 0 or less)\n";
				else
					std::cout << "\nInput spawn radius of galaxy: (number can NOT be 0 or less)\n";

				GETLINE;

				if (LINETOFLOAT > 0)
				{
					config.galaxyParam2D[i].radius = LINETOFLOAT;
					file << "\n#radius\n" << config.galaxyParam2D[i].radius;
					break;
				}
				std::cout << "\nEnter valid input:  (number can NOT be 0 or less)\n";
			}

			std::cout << "\nInput galaxy's intital center X location:\n";
			GETLINE;
			config.galaxyParam2D[i].centerLocation.x = LINETOFLOAT;
			file << "\n@@0" << "\n#centerLocation\n" << config.galaxyParam2D[i].centerLocation.x;

			std::cout << "\nInput galaxy's intital center Y location:\n";
			GETLINE;
			config.galaxyParam2D[i].centerLocation.y = LINETOFLOAT;
			file << "\n@@1\n" << config.galaxyParam2D[i].centerLocation.y;

			while(true)
			{
				std::cout << "\nEnable dynamic spiral (Y), or not (N): (Y/N) (/about for more information)\n";
				GETLINE;

				if (line.compare("/about") == 0)
				{
					std::cout << "\nABOUT: Dynamic spiral slows particle's inital tangent velocity according to the particle's distance from the center of the galaxy.\n This prevents a wierd hole in the middle of the galaxy from forming.\n";
				}
				else
				{
					if (line[0] == 'Y' || line[0] == 'y')
					{
						config.galaxyParam2D[i].dynamicSpiral = true;
						file << "\n#dynamicSpiral\nT";
						break;
					}
					if (line[0] == 'N' || line[0] == 'n')
					{
						config.galaxyParam2D[i].dynamicSpiral = false;
						file << "\n#dynamicSpiral\nF";
						break;
					}

					std::cout << "\nEnter valid input: (must be \'Y\', or \'N\')";
				}
			}

			while (true)
			{
				std::cout << "\nEnable dynamic expansion (Y), or not (N): (Y/N) (/about for more information)\n";
				GETLINE;

				if (line.compare("/about") == 0)
				{
					std::cout << "\nABOUT: Dynamic expansion slows particle's inital outward/inward velocity according to the particle's distance from the center of the galaxy.\n This prevents a wierd hole in the middle of the galaxy from forming.\n";
				}
				else
				{
					if (line[0] == 'Y' || line[0] == 'y')
					{
						config.galaxyParam2D[i].dynamicExpansion = true;
						file << "\n#dynamicExpansion\nT\n";
						break;
					}
					if (line[0] == 'N' || line[0] == 'n')
					{
						config.galaxyParam2D[i].dynamicExpansion = false;
						file << "\n#dynamicExpansion\nF\n";
						break;
					}

					std::cout << "\nEnter valid input: (must be \'Y\', or \'N\')";
				}
			}
		}
	}

	std::cout << "\nCONTROLS:\n";

	while (true)
	{
		std::cout << "\nInput initial zoom level: (1.0 = 100% zoom, 2.5 = 250% zoom) (this can be adjusted later with the scroll wheel)\n";
		GETLINE;

		if (LINETOFLOAT > 0)
		{
			config.defaultZoom = LINETOFLOAT;
			file << "\n#defaultZoom\n" << config.defaultZoom;
			break;
		}
		std::cout << "\nEnter valid input: (number can NOT be 0 or less)\n";
	}

	if (config.threeDimensions)
	{
		while (true)
		{
			std::cout << "\nInput orbit sensitivity: (orbit the origin in the simulation by holding down m1 and moving mouse)\n";
			GETLINE;

			if (LINETOFLOAT > 0)
			{
				config.orbitSensitivity = LINETOFLOAT;
				file << "\n#orbitSensitivity\n" << config.orbitSensitivity;
				break;
			}
			std::cout << "\nEnter valid input: (number can NOT be 0 or less)\n";
		}
	}

	std::cout << "\nConfiguration complete!\n";
	file.close();
	
	return;
}