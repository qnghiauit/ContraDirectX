//==============================
// Last change: 29/9/2014
// T6_Framework
// T6_Sound v1.v
// Store the class help you to load and play a wav audio.
// 29/9/2014: Add the cache system, allow reuse the loaded audio data.
//==============================
#ifndef __SOUND_H__
#define __SOUND_H__

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

#include <dsound.h>
#include <vector>
#include <memory>

#include "DefaultConstant.h"
#include "DefaultStructure.h"
#include "DSutil.h"


// -----------------------------------------------
// Name: class T6_Sound
// Desc: used to load/ store/ play an audio with wav extension.
// -----------------------------------------------
class Sound
{
public:
	Sound(const tchar* audioPath);
	~Sound(void);

	static HRESULT initializeSoundClass(HWND windowsHandler);
	static HRESULT releaseSoundClass();

	HRESULT play(bool isLoop = false, DWORD priority = 0);
	HRESULT stop();


private:
	HRESULT loadAudio(const tchar* audioPath);

private:
	static WAVEFORMATEX _bufferFormat;
	static DSBUFFERDESC _bufferDescription;
	static LPDIRECTSOUND8 _audioHandler;
	static HWND _windowsHandler;

	LPDIRECTSOUNDBUFFER _soundBuffer;
};

#endif