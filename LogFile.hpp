#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <ctime>

class LogFile
{
public:
	LogFile(std::string log_name);

	template<typename A>
	void log(const A& message) {
		log_file << message << std::endl;
	}

	template<typename A, typename ...Args>
	void log(const A& message, Args... others) {
		log_file << message << " ";
		this->log(others...);
	}

private:
	std::ofstream log_file;
};