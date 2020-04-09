#pragma once // microsoft include guard for visual studio.
#include "Windows.h" // needed for timer ops
// XTime is a timer class desingned to be used by D3D11 grahpics applications.(use one per thread)
// Use it for tracking time intervals in seconds with double percision.
// It also supports weighted time smoothing for time based movement. (should not be used for tracking time)
// Future versions may support uploading time data across multiple threads. (data sent to seperate thread profiler)
// Author: L.Norri CD DRX FullSail University
// Version: 1.2 - 1/21/2014  
class XTime
{
	// per thread timing data
	struct THREAD_DATA
	{
		LARGE_INTEGER signals[256], frequency, start;
		double totalTime, deltaTime, smoothDelta, blendWeight;
		double samplesPerSecond, lastSecond, actualHz;
		unsigned int threadID, elapsedSignals; 
		unsigned char signalCount, numSamples;
	}localStack; // instance of timing data on this thread
	
public:
	// Initialize the timer, 
	// samples tracks how many previous signals are used for weighted delta smoothing.
	// smoothFactor is used to assist computing a weighted average.
	// It should be a value less than one. it indicates the rate of falling importance of following samples.
	// Ex: 0.5 would mean that the second sample would be 50% less important the the first and the third would  be 25% and so on. 
	XTime(unsigned char samples = 10, double smoothFactor = 0.75);
	// Restarts the timer on the current thread (will clear all stored signals and total time)
	void Restart();
	// How Much time since the timer was restarted and the last call to signal?(fast)
	double TotalTime(); // will be the same per frame
	// How Much time has passed exactly since the timer was restarted?(slower)
	double TotalTimeExact(); // will be different between calls
	// This marks the cycling time period, typically called only once per frame per thread.
	void Signal();
	// what is the change in time? (between last 2 signals on this thread)
	double Delta();
	// Because the delta is one frame behind and not a linear change, visual motion can appear rough compared to fixed time steps.  
	// This method can be used to reterive a more granular version of the delta.(Better for motion)
	// This is acheived by using previous Delta values to compute a weighted running average.
	double SmoothDelta();
	// Returns the predicted rate of samples every second. Otherwise known as "FrameRate"
	// This value is re-evaluated 100 times per second if possible.  
	double SamplesPerSecond();
	// Use the "targetHz" parameter to enable thread throttling.
	// By default, thread throttling is not enabled "0". However by specifying a non-zero targetHz,
	// the Throttle function will attempt to gradually adjust the threads speed to match the target Hz by calling "Sleep".
	// this function is best called once per frame per thread just like "Signal"
	// ************************************* IMPORTANT *****************************************************************
	// By default the fidelity of the thread scheduler is 15.6ms in windows.
	// This means two "throttled" threads will likely not be able to communicate any faster than 32-64Hz.
	// You can improve thread scheduling response (up to 500Hz) by calling the windows function
	// "timeBeginPeriod(1)" at the start of your application followed by "timeEndPeriod(1)" at the end.
	// Be aware this is a global setting and will increase total power consumption roughly 25%.
	// If possible use an event/message based system instead for high speed threads. (favor using "Sync" below if possible)
	void Throttle(double targetHz);
	// Future Version: to be defined alongside a synchronization primitive.
	// Sync(???) // this may also happen in signal itself (not decided) but a bitwise register/unregister system is likely.
	// Will cause a thread to synchronize to the signal count of another thread(s).
	// Will work for multiple threads and always sync to the slowest in the group.(support offsets)
};