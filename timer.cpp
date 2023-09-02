#include "timer.h"

#include <iostream>

#if PERFORMANCE_LOGS == 1
timer::timer(std::string input) {
	begin = std::chrono::high_resolution_clock::now();
	printMessage = input;
}
timer::~timer() {
	std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - begin;

	std::cout << printMessage << ": " << duration.count() << "\n";
}
#else
timer::timer(std::string input){}
timer::~timer() {}
#endif