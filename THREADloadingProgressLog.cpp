#include <iostream>
#include <thread>
#include <chrono>

void THREADloadingProgressLog(unsigned long* numberSpawned, unsigned int total)
{

	int lastTotaltotalGenerated = -1;
	while (true)
	{
		if (*numberSpawned == lastTotaltotalGenerated)
		{
			std::cout << "Particles spawned: " << total << "/" << total << "\n" << 100 << "% complete\n";
			std::cout << "Done!\n";
			return;
		}
		std::cout << "Particles spawned: " << *numberSpawned << "/" << total << "\n" << (float)(*numberSpawned) / total * 100 << "% complete\n";
		lastTotaltotalGenerated = *numberSpawned;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	return;
}