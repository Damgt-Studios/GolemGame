#include "pchgame.h"
#include "XTime.h"
#include <math.h>

XTime::XTime(unsigned char samples, double smoothFactor)
{
	// clear the structure and init basic values
	ZeroMemory(&localStack,sizeof(THREAD_DATA));
	localStack.numSamples =  max(1,samples);// one sample is minimum
	localStack.blendWeight = smoothFactor;
	localStack.threadID = GetCurrentThreadId();
	// Thread & frame rate measurements (used for throttling)
	localStack.samplesPerSecond = localStack.lastSecond = 0; 
	localStack.actualHz = 0;
	Restart();
}
void XTime::Restart()
{
	// get processor frequency (length of each tick on this core)
	QueryPerformanceFrequency( &localStack.frequency );
	// reset counters
	localStack.deltaTime = localStack.totalTime = 
	localStack.smoothDelta = localStack.lastSecond = 0.0;
	localStack.signalCount = localStack.elapsedSignals = 0;
	// Track the start time
    QueryPerformanceCounter( &localStack.start ); 
	localStack.signals[localStack.signalCount++] = localStack.start;
}
double XTime::TotalTime()
{
	return localStack.totalTime;
}
double XTime::TotalTimeExact()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter( &now ); // what is the time right now?
	LONGLONG elapsed = now.QuadPart - localStack.start.QuadPart; // determine time elapsed since the start.
	return double(elapsed) / double(localStack.frequency.QuadPart); // return in seconds
}
// Append to the signal buffer and compute resulting times
void XTime::Signal()
{
	// make room for the new signal
	memmove_s(localStack.signals+1u, sizeof(LARGE_INTEGER) * 255, localStack.signals, sizeof(LARGE_INTEGER) * localStack.numSamples); 
	// append to the front of signals and up the count (no more than the last index tho)
	QueryPerformanceCounter( localStack.signals );
	localStack.signalCount = min( localStack.signalCount+1, 255 );
	// with our signal buffer updated, we can now compute our timing values
	localStack.totalTime = double((*localStack.signals).QuadPart - localStack.start.QuadPart) / double(localStack.frequency.QuadPart);
	localStack.deltaTime = double(localStack.signals[0].QuadPart - localStack.signals[1].QuadPart) / double(localStack.frequency.QuadPart);
	// with our signal buffer updated we can compute our weighted average for a smoother delta curve.
	double totalWeight = 0, runningWeight = 1;
	LONGLONG totalValue = 0, sampleDelta;
	// loop up to num samples or as many as we have available
	for(unsigned char i = 0; i < min(localStack.numSamples, localStack.signalCount-1); ++i)
	{
		// determine each delta as we go
		sampleDelta = localStack.signals[i].QuadPart - localStack.signals[i+1].QuadPart;
		totalValue += LONGLONG(sampleDelta * runningWeight); // this cast is expensive, need to look into optimizing
		totalWeight += runningWeight; // tally all the weights used
		runningWeight *= localStack.blendWeight; // adjust the weight of next delta
	}
	// with our totals calculated, determine the weighted average.
	localStack.smoothDelta = (totalValue / totalWeight) / double(localStack.frequency.QuadPart);
	
	++localStack.actualHz;
	
	// done calculating deltas, now determine frame rate. (signals-per-second)
	++localStack.elapsedSignals; // track passed signals
	double sinceLast = localStack.totalTime - localStack.lastSecond; 
	if(sinceLast >= 0.1) // update 10 times per second if possible
	{ 
		localStack.samplesPerSecond = localStack.elapsedSignals / sinceLast; 
		localStack.lastSecond = localStack.totalTime; 
		localStack.elapsedSignals = 0;
	}// done
}
double XTime::Delta()
{
	return localStack.deltaTime;
}
double XTime::SmoothDelta()
{
	return localStack.smoothDelta;
}
double XTime::SamplesPerSecond()
{
	return localStack.samplesPerSecond;
}
// Slow down or speed up a thread to match a desired cycle rate(Hz)
// Ver 1.2: Now utilizes 10Hz FPS counter for more granular slow down.
void XTime::Throttle(double targetHz) 
{	
	if(targetHz > 1) // SOLVED!!!!!!
	{
		// if we are going too fast slow down
		unsigned int slow = 0;
		while(localStack.elapsedSignals / (TotalTimeExact() - localStack.lastSecond) > targetHz) 
			Sleep(slow++);
	}	
}