#include "FreeSound.h"


FreeSound* FreeSound::_instance = nullptr;


FreeSound::FreeSound()
{
}


FreeSound::~FreeSound()
{
	FOR_A(_loadedSound.begin(), _loadedSound.end())
		SAFE_RELEASE(i->second);
	_loadedSound.clear();
}

void FreeSound::initialize(HWND windowsHandler)
{
	Sound::initializeSoundClass(windowsHandler);
}

void FreeSound::play(const tchar* soundLink, bool repeat /*= false*/, bool playFromStart /*= true*/)
{
	play(tstring(soundLink), repeat, playFromStart);
}

void FreeSound::play(tstring soundLink, bool repeat /*= false*/, bool playFromStart /*= true*/)
{
	try
	{
		if (playFromStart)
			get()->_loadedSound.at(soundLink)->stop();

		get()->_loadedSound.at(soundLink)->play(repeat);
	}
	catch (exception)
	{
		get()->_loadedSound.insert(SoundPair(soundLink, new Sound(soundLink.c_str())));
		get()->_loadedSound.at(soundLink)->play(repeat);
	}
}

FreeSound* FreeSound::get()
{
	if (_instance == nullptr)
	{
		_instance = new FreeSound();
	}

	return _instance;
}

void FreeSound::release()
{
	SAFE_RELEASE(_instance);
	Sound::releaseSoundClass();
}

void FreeSound::stop(const tchar* soundLink)
{
	stop(tstring(soundLink));
}


void FreeSound::stop(tstring soundLink)
{
	try
	{
		get()->_loadedSound.at(soundLink)->stop();
	}
	catch (exception)
	{

	}
}
