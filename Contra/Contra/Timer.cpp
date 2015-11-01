#include "Timer.h"

#if MEMORY_LEAK_DEBUG == 1
#include <vld.h>
#endif

Timer* Timer::_instance = 0;

Timer::Timer()
{
	_lastTimeSign = clock();
	_currentTimeSign = _lastTimeSign;
	_prevRoundTime = 0;
	_roundCode = 0;
}


Timer::~Timer()
{
	SAFE_RELEASE(_instance);
}


Timer& Timer::getInstance()
{
	if (_instance == 0)
		_instance = new Timer();

	return *_instance;
}


DWORD Timer::getRoundTime()
{
	if (Camera::getState() == CAMERA_AUTO)
	{
		return 0;
	}
	return Timer::getInstance()._prevRoundTime;
}


void Timer::updateTimeSign()
{
	clock_t*	lastTimeSign	= &Timer::getInstance()._lastTimeSign;
	clock_t*	currentTimeSign = &Timer::getInstance()._currentTimeSign;
	DWORD*		prevRoundTime	= &Timer::getInstance()._prevRoundTime;

	*lastTimeSign		= *currentTimeSign;
	*currentTimeSign	= clock();
	*prevRoundTime		= *currentTimeSign - *lastTimeSign;
	Timer::getInstance()._roundCode = (Timer::getInstance()._roundCode + 1) % 100;
}


void Timer::updateRoundTime()
{
	Timer::getInstance()._prevRoundTime = (DWORD)(clock() - Timer::getInstance()._currentTimeSign);
}


int Timer::getRoundCode()
{
	return getInstance()._roundCode;
}

DWORD Timer::getCameraRoundTime()
{
	return Timer::getInstance()._prevRoundTime;
}
