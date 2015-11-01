//==============================
// Last change: 9/10/2014
// T6_Framework
// T6_Sprite v1.1
// This class's objects store a texture and num of columns, rows to help you
// draw the image on screen frame-per-frame for animation.
// 28/9/2014: remove Name, and change Path type to const tchar*.
// 9/10/2014: add a new properties for the time to switch frames.
//==============================
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <fstream>
#include <time.h>

#include "DefaultStructure.h"
#include "DefaultConstant.h"
#include "DefaultMethods.h"
#include "Texture.h"

typedef int(*getInt)();
typedef DWORD(*getDword)();

class Sprite : public Texture
{
public:
	Sprite():Texture(){};
	Sprite (const tchar* spritePatch,
				int nColumns, int nRows, int totalframe = 0, int CrF = 0);
	Sprite(const Sprite&	sourceSprite);
	Sprite(const Texture&	sourceTexture,
				const int& nColums = 1, const int& nRows = 1);
	Sprite(const tchar* path, fstream& infoFile);
	virtual ~Sprite(void);

	static void	synchronousProcessToNextFrame(double passedGameTime);

	bool	processToNextFrame(double passedGameTime);
	void	setFrameTime(double newTime);
	void	setFrame(const int& currentFrame = 0);
	double	getFrameTime();
	RECT	getSrcRect();
	int		getColumns();
	Size	getFrameSize();
	void	reset();
	bool	loadSprite();
	bool	isComplete();
	void	setAutoSwitchFrame(bool isOn);
	Sprite*	 clone()			const;
	unsigned int getCurrentFrame()	const;
	unsigned int getTotalFrames()	const;


	bool render(double		passedGameTime = 0.0,
				D3DXVECTOR3	position	= D3DXVECTOR3(0,0,0.5),
				float		angle		= ANGLE_NO,
				D3DXVECTOR2	scale		= SCALE_NO,
				D3DXVECTOR2	translate	= TRANSLATE_NO,
				DrawCenter	drawCenter	= DrawCenter::CENTER,
				D3DCOLOR	color		= D3DCOLOR_XRGB(255,255,255),
				int			from		= 0,
				int			to			= -1,
				RECT*		sourceRect	= nullptr);


	bool render(double		passedGameTime,
				D3DXVECTOR3	position,
				float		angle,
				D3DXVECTOR2	scale,
				D3DXVECTOR2	translate,
				D3DXVECTOR3 drawCenter,
				D3DCOLOR	color = D3DCOLOR_XRGB(255, 255, 255),
				int			from = 0,
				int			to = -1,
				RECT*		sourceRect = nullptr);


	bool render(double		passedGameTime = 0.0,
				D3DXVECTOR3	position = D3DXVECTOR3(0, 0, 0.5),
				float		visiblePercent_width = 1.0f,
				float		visiblePercent_height = 1.0f,
				bool		isTranslateXAxis = true,
				D3DXVECTOR2	scale = SCALE_NO,
				DrawCenter	drawCenter = DrawCenter::CENTER,
				int			from = 0,
				int			to = -1);

	Sprite& operator= (Sprite sourceSprite);

	static void initialize(getInt getTimeCodeFunction, getDword getRoundTimeFunction);
	static void update();

private:
	double _switchFrameTime;
	double _fromLastSwitch;
	int		_nColumns,		_nRows;
	int		_currentFrame,	_nFrame;
	bool	_isComplete;
	Size	_frameSize;
	bool	_autoSwitchFrame;
	int		_switchStep;

	int		_currentRenderStartPoint;
	int		_currentRenderStopPoint;

	void caculateFrameSize();

	static vector<Sprite*> _synchronousSpriteList;
	static int _defaultIndex;
	static int _timeCode;
	static DWORD _timer;
	static getInt _getTimeCode;
	static getDword _getRoundTime;
};

#endif