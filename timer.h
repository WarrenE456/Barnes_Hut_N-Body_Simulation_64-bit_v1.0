#pragma once

#include <chrono>
#include <string>

class timer {

public:
	std::chrono::steady_clock::time_point begin;
	std::string printMessage;

	timer(std::string input);
	~timer();
};