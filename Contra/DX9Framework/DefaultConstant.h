#ifndef __DEFAULT_CONSTANT_H__
#define __DEFAULT_CONSTANT_H__

#include <Windows.h>
#include <tchar.h>
#include <typeinfo>

#define MEMORY_LEAK_DEBUG		0

// Window default information.
#define RESOLUTION_WIDTH		256
#define RESOLUTION_HEIGHT		240
#define SCREEN_WIDTH			1024
#define SCREEN_HEIGHT			600
#define RESOLUTION_CURSOR_W		50
#define RESOLUTION_CURSOR_H		50
#define RESOLUTION_ICON_W		256
#define RESOLUTION_ICON_H		256
#define SIZE_TITLEBAR			31
#define SIZE_BORDER				8
#define DEFAULT_DEPTH			0.0f

// Time to change frame in sprite sheet.
#define DEFAULT_FRAME_TIME		(1000.0f / 10)

// Default audio setting.
#define AUDIO_FORMAT_TAG		WAVE_FORMAT_PCM
#define	AUDIO_NUM_OF_CHANNEL	2
#define AUDIO_SAMPLE_SPEED		22050
#define AUDIO_BITS_PER_SAMPLE	16
#define AUDIO_FLAGS				0
#define AUDIO_BUFFER_SIZE		640000
#define AUDIO_GUID				GUID_NULL

#define AUDIO_BLOCK_ALIGN(bitPerSample, nChannels)		(WORD)(bitPerSample / 8 * nChannels)
#define AUDIO_AVERAGE_BPS(samplesPerSec, blockAlign)	(DWORD)(samplesPerSec * blockAlign)

// Default GUI setting.
#define DEFAULT_CURSOR		IDC_ARROW
#define DEFAULT_ICON		IDI_SHIELD

// Default text setting.
#define DEFAULT_FONTSIZE	8
#define DEFAULT_FONTFACE	_T("Contra (Famicom/NES)")
#define DEFAULT_FONTPATH	_T("Resource//font.ttf")

// Specify position on screen.
#define WDPOS_CENTER		(D3DXVECTOR3(RESOLUTION_WIDTH / 2.0F,	RESOLUTION_HEIGHT / 2.0F,	DEFAULT_DEPTH))
#define WDPOS_NOTIFY		(D3DXVECTOR3(RESOLUTION_WIDTH / 2.0F,	RESOLUTION_HEIGHT / 4.0F,	DEFAULT_DEPTH))
#define WDPOS_BOTTOM_CENTER (D3DXVECTOR3(RESOLUTION_WIDTH / 2.0F,	RESOLUTION_HEIGHT,			DEFAULT_DEPTH))
#define WDPOS_BOTTOM_RIGHT	(D3DXVECTOR3(RESOLUTION_WIDTH,			RESOLUTION_HEIGHT,			DEFAULT_DEPTH))
#define WDPOS_BOTTOM_LEFT	(D3DXVECTOR3(0.0F,						RESOLUTION_HEIGHT,			DEFAULT_DEPTH))
#define WDPOS_TOP_LEFT		(D3DXVECTOR3(0.0F,						0.0F,						DEFAULT_DEPTH))
#define WDPOS_CENTER_(A)	(D3DXVECTOR3(RESOLUTION_WIDTH / 2.0F,	100 + 80 * A,				DEFAULT_DEPTH))
#define POSITION_ZERO		(D3DXVECTOR3(0.0F,						0.0F,						0.0F))

// Specify position in game world displayed on screen.
#define GAMEPOS_CENTER			WDPOS_CENTER
#define GAMEPOS_NOTIFY			(D3DXVECTOR3(RESOLUTION_WIDTH / 2.0F,	RESOLUTION_HEIGHT / 4.0F * 3,	DEFAULT_DEPTH))
#define GAMEPOS_BOTTOM_CENTER	(D3DXVECTOR3(RESOLUTION_WIDTH / 2.0F,	0.0F,							DEFAULT_DEPTH))
#define GAMEPOS_BOTTOM_RIGHT	(D3DXVECTOR3(RESOLUTION_WIDTH,			0.0F,							DEFAULT_DEPTH))
#define GAMEPOS_BOTTOM_LEFT		(D3DXVECTOR3(0.0F,						0.0F,							DEFAULT_DEPTH))
#define GAMEPOS_TOP_LEFT		(D3DXVECTOR3(0.0F,						RESOLUTION_HEIGHT,				DEFAULT_DEPTH))
#define GAMEPOS_TOP_CENTER		(D3DXVECTOR3(RESOLUTION_WIDTH / 2.0F,	RESOLUTION_HEIGHT,				DEFAULT_DEPTH))

// Specify colors.
#define COLOR_NO			D3DCOLOR_XRGB(255,	255,	255)
#define COLOR_WHITE			D3DCOLOR_XRGB(255,	255,	255)
#define COLOR_BLACK			D3DCOLOR_XRGB(0,	0,		0)

// About zoom macros.
#define SCALE_NO		D3DXVECTOR2(1.0F, 1.0F)
#define SCALE_FLIP_X	D3DXVECTOR2(-1.0F, 1.0F)
#define SCALE_FLIP_Y	D3DXVECTOR2(1.0F, -1.0F)
#define SCALE_FLIP_XY	D3DXVECTOR2(-1.0F, -1.0F)
#define SCALE_A(A)		D3DXVECTOR2(A, A)
#define SCALE_XY(A, B)	D3DXVECTOR2(A, B)
#define SCALE_VELO_X(A)	D3DXVECTOR2(A != 0? (float)A/fabs(A) : 1.0F, 1.0F)

// Define to fast change a vector.
#define FLIP_Y(vector3)		(D3DXVECTOR3(vector3.x, -vector3.y, vector3.z))
#define FLIP_X(vector3)		(D3DXVECTOR3(-vector3.x, vector3.y, vector3.z))

// Simple functions
#define MAX(A, B)			(A > B ? A : B)
#define MIN(A, B)			(A < B ? A : B)
#define MAX_ABS(A, B)		(fabs(A) > fabs(B) ? A : B)
#define MIN_ABS(A, B)		(fabs(A) < fabs(B) ? A : B)

// Translating macro.
#define TRANSLATE_NO		D3DXVECTOR2(0.0f, 0.0f)

// Move speed macros.
#define TRUE_SPEED(speed,time)	(speed * (time / 1000.0F))
#define SPEED_NO			D3DXVECTOR3(0.0f,	0.0f,	0.0f)
#define SPEED_X(A)			D3DXVECTOR3(A,		0.0f,	0.0f)
#define SPEED_Y(A)			D3DXVECTOR3(0.0f,	A,		0.0f)	
#define SPEED_XY(A, B)		D3DXVECTOR3(A,		B,		0.0f)

// Simple defination For-loop macros.
#define FOR(A)				for(decltype(A) i = 0; i < (A); ++i)
#define FOR_e(A)			for(int i = 0; i < (A); ++i)
#define DFOR(A)				for(int i = (A); i >= 0; --i)
#define FORJ(A)				for(auto j = (A) - (A); j < (A); ++j)
#define FOR_A(start,stop)	for(auto i = start; i != stop; ++i)
#define FOR_AB(A, B)		for(auto i = (A); i < (B); ++i)

// Safe delete pointer macro.
#define SAFE_RELEASE(A)		{if (A) {delete A; A = 0;}}

#define ASSIGN_PTR(ptr, value) {if (ptr != nullptr) {*ptr = value;}}

#define WARNING_BOX(A)		MessageBox(0, A, _T("Error"), MB_OK)

// Warning descriptiones==============================
#ifdef UNICODE

#define WARNING_CANNOT_CREATE_WINDOWS		L"Không thể tạo cửa sổ.\n"
#define WARNING_CANNOT_LOAD_IMAGE			L"\nKhông thể load được hình ảnh này.\n"
#define WARNING_CANNOT_LOAD_SPRITE			L"Không thể load hoạt ảnh.\n"
#define WARNING_UNDETECTED_DEVICE			L"Không tồn tại thiết bị vẽ khả dụng.\n"
#define WARNING_CANNOT_START_KEYBOARD		L"Không khởi tạo được thiết bị nhập.\n"
#define WARNING_UNAVAILABLE_AUDIO_DEVICE	L"Không khởi tạo được thiết bị kết xuất âm thanh.\n"
#define WARNING_CANNOT_OPEN_FILE			L"Tập tin không tồn tại.\n"
#define WARNING_CANNOT_READ_KEYBOARD		L"Không thể nhận dữ liệu bàn phím.\n"

#else

#define WARNING_CANNOT_CREATE_WINDOWS		"Cannot create windows.\n"
#define WARNING_CANNOT_LOAD_IMAGE			"Unable to load this image.\n"
#define WARNING_CANNOT_LOAD_SPRITE			"Unable to load this sprite.\n"
#define WARNING_UNDETECTED_DEVICE			"Display device haven't been declared.\n"
#define WARNING_CANNOT_START_KEYBOARD		"Unable to initialize keyboard.\n"
#define WARNING_UNAVAILABLE_AUDIO_DEVICE	"Audio device are unavailable.\n"
#define WARNING_CANNOT_OPEN_FILE			"Undefined file.\n"
#define WARNING_CANNOT_READ_KEYBOARD		"Unable to read keyboard status.\n"

#endif

#endif