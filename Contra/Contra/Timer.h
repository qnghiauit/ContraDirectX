#ifndef __TIMER_H__
#define __TIMER_H__

#include <time.h>

#include "DX9Framework\DefaultConstant.h"
#include "DX9Framework\Camera.h"

class Timer
{
public:
	static DWORD	getRoundTime();
	static DWORD	getCameraRoundTime();
	static void		updateTimeSign();
	static void		updateRoundTime();
	static int		getRoundCode();

private:
					Timer();
					~Timer();
	static Timer&	getInstance();

private:
	static	Timer*	_instance;
			clock_t	_lastTimeSign;
			clock_t	_currentTimeSign;
			DWORD	_prevRoundTime;
			int		_roundCode;
};

#endif