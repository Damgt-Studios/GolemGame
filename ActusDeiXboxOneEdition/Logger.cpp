#include "pch.h"
#include "Logger.h"	

string Logger::buffer = "";
string Logger::newMessage = "";

time_t Logger::current_time = time(NULL);
time_t Logger::message_time = time(NULL) -10;

time_t Logger::prev_time = time(NULL) -60 ;
bool sent = false;
void Logger::log(string message = "")
{
	Logger::current_time = time(NULL);

	Logger::newMessage = "";
	char str_time[26];
	ctime_s(str_time, sizeof (str_time), &current_time);
	Logger::newMessage.append(str_time);
	Logger::newMessage.append(message);
	Logger::newMessage.append("\n");

	if (difftime(Logger::current_time, Logger::message_time) >= 10)
	{
		Logger::message_time = Logger::current_time;

		buffer.append(newMessage);
	}

	if (difftime(Logger::current_time, Logger::prev_time) >= 60)
	{
		Logger::prev_time = Logger::current_time;

		const char* path = "debug.Log";
		ofstream file(path, std::ios::app | std::ios::out);

		assert(file.is_open());


		file << buffer.data();

		file.close();

		buffer.clear();




	}



};


