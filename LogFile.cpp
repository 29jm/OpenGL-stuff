#include "LogFile.hpp"

using namespace std;

LogFile::LogFile(string log_name)
	: log_file(log_name)
{
	if (!log_file)
	{
		cerr << "ERROR: could not open the log file "
			 << log_name << " for writing" << endl;
	}

	time_t now = time(nullptr);
	string date(ctime(&now));

	this->log("Starting log file", log_name, ". Local time", date);
}
