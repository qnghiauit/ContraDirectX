#ifndef __FREE_SOUND_H__
#define __FREE_SOUND_H__

#include "DX9Framework/Sound.h"

#include <map>

typedef map<tstring, Sound*> SoundMap;
typedef pair<tstring, Sound*> SoundPair;

class FreeSound
{
public:
	static void initialize(HWND windowsHandler);
	
	static void release();

	static void play(const tchar* soundLink, bool repeat = false, bool playFromStart = true);

	static void play(tstring soundLink, bool repeat = false, bool playFromStart = true);
	
	static void stop(const tchar* soundLink);

	static void stop(tstring soundLink);

private:
	FreeSound();
	~FreeSound();
	static FreeSound* get();

private:
	static FreeSound* _instance;

	SoundMap _loadedSound;
};

#endif // FreeSound_h__
