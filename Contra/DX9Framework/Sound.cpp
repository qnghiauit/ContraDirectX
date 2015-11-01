#include "Sound.h"

WAVEFORMATEX Sound::_bufferFormat;
DSBUFFERDESC Sound::_bufferDescription;
LPDIRECTSOUND8 Sound::_audioHandler;
HWND Sound::_windowsHandler;

// -----------------------------------------------
// Desc: Get the audio Name and Path, ready to load.
// -----------------------------------------------
Sound::Sound(const tchar* audioPath)
{
	loadAudio(audioPath);
}



Sound::~Sound(void)
{
	_soundBuffer->Stop();
}


// -----------------------------------------------
// Desc: Initialize the basic PROPERTIESs for loading audio
// -----------------------------------------------
HRESULT Sound::initializeSoundClass(HWND windowsHandler)
{
	_windowsHandler = windowsHandler;

	HRESULT result;
	result = DirectSoundCreate8(0, &_audioHandler, 0);
	result = result | _audioHandler->SetCooperativeLevel(_windowsHandler, DSSCL_PRIORITY);

	ZeroMemory(&_bufferFormat, sizeof(WAVEFORMATEX));
	ZeroMemory(&_bufferDescription, sizeof(DSBUFFERDESC));

	_bufferFormat.wFormatTag		= AUDIO_FORMAT_TAG;
	_bufferFormat.nChannels			= AUDIO_NUM_OF_CHANNEL;
	_bufferFormat.nSamplesPerSec	= AUDIO_SAMPLE_SPEED;
	_bufferFormat.wBitsPerSample	= AUDIO_BITS_PER_SAMPLE;
	_bufferFormat.nBlockAlign		= AUDIO_BLOCK_ALIGN(_bufferFormat.wBitsPerSample,
		_bufferFormat.nChannels);
	_bufferFormat.nAvgBytesPerSec	= AUDIO_AVERAGE_BPS(_bufferFormat.nSamplesPerSec,
		_bufferFormat.nBlockAlign);

	_bufferDescription.dwFlags			= AUDIO_FLAGS;
	_bufferDescription.guid3DAlgorithm	= AUDIO_GUID;
	_bufferDescription.dwSize			= sizeof(DSBUFFERDESC);

	return result;
}


// -----------------------------------------------
// Desc: Release the basic PROPERTIES after used (close game).
// -----------------------------------------------
HRESULT Sound::releaseSoundClass()
{
	if (_audioHandler != 0)
		return _audioHandler->Release();

	return S_OK;
}


// -----------------------------------------------
// Desc: Load the Audio stored in audioPath.
// -----------------------------------------------
HRESULT Sound::loadAudio(const tchar* audioPath_)
{
	HRESULT result;
	CWaveFile audioObject;
	result = audioObject.Open(LPTSTR(audioPath_), 0, WAVEFILE_READ);

	if (!FAILED(result)) {

		_bufferDescription.dwBufferBytes = audioObject.GetSize();
		_bufferDescription.lpwfxFormat = audioObject.m_pwfx;

		result = _audioHandler->CreateSoundBuffer(&_bufferDescription, &_soundBuffer, 0);

		VOID* pointerToLockedBuffer = 0;
		DWORD lockedSize = 0;
		result = result | (_soundBuffer)->Lock(0, AUDIO_BUFFER_SIZE, &pointerToLockedBuffer,
			&lockedSize, 0, 0, DSBLOCK_ENTIREBUFFER);

		if (!FAILED(result)) {
			DWORD readedData = 0;
			audioObject.ResetFile();
			result = audioObject.Read((BYTE*)pointerToLockedBuffer, lockedSize, &readedData);
			if (!FAILED(result)) {
				(_soundBuffer)->Unlock(pointerToLockedBuffer, lockedSize, 0, 0);
			}
		}
	}

	audioObject.Close();
	return result;
}


// -----------------------------------------------
// Desc: Play loaded audio, may choose loop or no.
// -----------------------------------------------
HRESULT Sound::play(bool isLoop, DWORD priority)
{
	return _soundBuffer->Play(0, priority, isLoop & DSBPLAY_LOOPING);
}


// -----------------------------------------------
// Desc: Stop the audio if it is playing.
// -----------------------------------------------
HRESULT Sound::stop()
{
	HRESULT result = _soundBuffer->Stop();
	_soundBuffer->SetCurrentPosition(0);
	return result;
}