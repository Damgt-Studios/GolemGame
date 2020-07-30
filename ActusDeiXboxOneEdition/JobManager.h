#pragma once



#include <thread>
#include <vector>
#include <atomic>

#include "concurrentqueue.h"

namespace Jobs 
{
	class JobManager
	{
	public:

		static JobManager* GetInstance();

		static void ReleaseInstance();

		void AddJob(void(*func_ptr)(void*, int), void* args, int index);

		template<class T>
		void AddJob(T* aParent,
			void(T::* func_ptr)(void*, int),
			void* args,
			int index)
		{
			CpuJob aJob = {};
			aJob.jobArgs = args;
			aJob.index = index;

			IJOB* jobPtr = new JobMemberFunc<T>(aParent, func_ptr);
			aJob.jobPtr = jobPtr;

			locklessQueue.enqueue(aJob);

		}

		JobManager(JobManager const&) = delete;

		void operator=(JobManager const&) = delete;

		//Accessories

		inline const size_t GetThreadCount() const;

		int GetAmountOfSuppoertedThreads();


	private:

		JobManager();

		~JobManager();

		static JobManager* instance;

		void WorkerThread();

		std::vector<std::thread> workerThreads;

		std::atomic<bool> isDone;

		struct IJOB
		{
			virtual ~IJOB() {}

			virtual bool invoke(void* args, int index) = 0;
		};

		struct JobFunc : IJOB
		{
			JobFunc(void(*aFunc_pointer)(void*, int)) : func_ptr(aFunc_pointer)
			{

			}

			virtual bool invoke(void* args, int index) override
			{
				func_ptr(args, index);
				return true;
			}

			void (*func_ptr)(void*, int);
		};

		template<class T>
		struct JobMemberFunc
		{
			JobMemberFunc(T* aParent, void(T::* f)(void*, int))
			{
				parentObj = aParent;
				func_ptr = f;
			}

			virtual bool invoke(void* args, int index) override
			{
				if (!parentObj) return false;

				(parentObj->*func_ptr)(args, index);

				return true;
			}

			T* parentObj;

			void(T::* func_ptr)(void*, int);

		};

		struct CpuJob
		{
			IJOB* jobPtr = nullptr;
			void* jobArgs = nullptr;
			int index = 0;
		};


		moodycamel::ConcurrentQueue<CpuJob> locklessQueue;
	};
}