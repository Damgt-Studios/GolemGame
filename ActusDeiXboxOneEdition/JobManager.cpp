#include "pch.h"
#include "JobManager.h"

using namespace Jobs;


JobManager* JobManager::instance = nullptr;

JobManager* JobManager::GetInstance()
{
	if (instance == nullptr)
		instance = new JobManager();

	return instance;
}

void JobManager::ReleaseInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

JobManager::JobManager()
{
	const unsigned int SupportedThreads = GetAmountOfSuppoertedThreads();

	isDone = false;

	for (size_t i = 0; i < SupportedThreads; i++)
	{
		workerThreads.push_back(std::thread(&Jobs::JobManager::WorkerThread, this));
	}
}

JobManager::~JobManager()
{
	isDone = true;

	for (auto &item : workerThreads)
	{
		item.join();
	}
}

void JobManager::AddJob(void(*func_ptr)(void*, int), void* args, int index)
{
	CpuJob aJob;
	aJob.jobArgs = args;
	aJob.index = index;

	IJOB* jobPtr = new JobFunc(func_ptr);
	aJob.jobPtr = jobPtr;

	locklessQueue.enqueue(aJob);
}

void JobManager::WorkerThread()
{
	while (true)
	{

		if (isDone) return;

		CpuJob currJob;
		bool found = locklessQueue.try_dequeue(currJob);

		if (found)
		{
			if (currJob.jobPtr)
			{
				currJob.jobPtr->invoke(currJob.jobArgs, currJob.index);

				delete currJob.jobPtr;
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(3));
		}

	}
}

//Accessories

inline const size_t JobManager::GetThreadCount() const
{
	return workerThreads.size();
}

inline int JobManager::GetAmountOfSuppoertedThreads()
{
	return std::thread::hardware_concurrency();
}