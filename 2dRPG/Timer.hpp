#pragma once
#include "SDL.h"

class Timer {
private:
	static Timer* sInstance;

	const int FPS = 60;
	const int FRAMEDELAY = 1000 / FPS;

	unsigned int mStartTicks;
	unsigned int mElapsedTicks;
	float mDeltaTime;
	float mTimeScale;

	Timer();
	~Timer();


public:
	static Timer* Instance();
	static void Release();

	void Reset();
	float DeltaTime();

	void TimeScale(float t);
	float TimeScale();
	int Fps();
	int FrameDelay();
	

	void Update();

};