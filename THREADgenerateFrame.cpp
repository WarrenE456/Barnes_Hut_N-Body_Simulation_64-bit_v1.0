#include "THREADgenerateFrame.h"

#include <fstream>
#include <string>

void THREADgenerateFrame(unsigned int frameCount, unsigned char* redChannel, unsigned char* blueChannel, unsigned char* greenChannel, unsigned int height, unsigned int width, std::string renderFilePath)
{
	std::fstream frame;
	frame.open(renderFilePath + "\\frame." + std::to_string(frameCount) + ".ppm", std::ios::out);

	if (!frame)
		return;

	frame << "P3\n" << width << " " << height << "\n255\n";

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			frame << (unsigned short)redChannel[width * h + w] << " " << (unsigned short)greenChannel[width * h + w] << " " << (unsigned short)blueChannel[width * h + w] << "\n";
		}
	}

	frame.close();

	return;
}