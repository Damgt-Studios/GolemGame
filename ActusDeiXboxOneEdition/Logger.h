#pragma once
#include <string>
#include <time.h>
#include <fstream>
#include <assert.h>

using namespace std;

class Logger
{
public:
	static string buffer;
	static string newMessage;
	static time_t prev_time;
	static time_t current_time;
	static time_t message_time;




	static void log(string message);
	

	Logger();



};

