#pragma once
#include <string>
#include <time.h>
//#include <fstream>
//#include <assert.h>
//#include <winrt\base.h>
//#include <winrt\Windows.Foundation.Collections.h>
//#include <winrt\Windows.Web.Syndication.h>
//#include <winrt\Windows.Storage.Streams.h>
//#include <winrt\Windows.Storage.h>
//#include <winrt\Windows.ApplicationModel.h>
//#include <iostream>

class Logger
{
public:
	static std::string buffer;
	static std::string newMessage;
	static time_t prev_time;
	static time_t current_time;
	static time_t message_time;

	static void LogMessage(std::string message = "")
	{
		//StorageFolder* storageFolder = ApplicationData::Current->LocalFolder;
		//concurrency::create_task(storageFolder->CreateFileAsync("sample.txt", CreationCollisionOption::ReplaceExisting));


	};
};

