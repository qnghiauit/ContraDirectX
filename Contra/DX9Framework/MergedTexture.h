#ifndef __MERGED_TEXTURE_H__
#define __MERGED_TEXTURE_H__

#include "Texture.h"

typedef int(*getInt)();
typedef DWORD(*getDword)();

#define DEFAULT_INDEX	-1

class MergedTexture : public Texture
{
public:
	MergedTexture(const tchar* spritePatch, int nColumns, int nRows);
	MergedTexture(const MergedTexture& sourceMTexture);
	virtual ~MergedTexture();

	virtual void draw  (index_t index,
				D3DXVECTOR3 position,
				DrawCenter drawCenter	= DrawCenter::TOPLEFT,
				D3DXVECTOR2 scale		= SCALE_NO,
				D3DXVECTOR2 translate	= TRANSLATE_NO,
				float angle				= ANGLE_NO,
				D3DCOLOR fadedColor		= 0xFFFFFFFF);

	virtual void draw(index_t index,
					  D3DXVECTOR3 position,
					  float percentOfVisible_width,
					  float percentOfVisible_height,
					  bool	isTranslateXAxis = true,
					  DrawCenter drawCenter	= DrawCenter::TOPLEFT,
					  D3DXVECTOR2 scale		= SCALE_NO,
					  D3DXVECTOR2 translate	= TRANSLATE_NO,
					  float angle			= ANGLE_NO,
					  D3DCOLOR fadedColor	= 0xFFFFFFFF);

	Size getFrameSize();

	void loadMergedTexture();
	MergedTexture* clone() const;
	static void initialize(getInt getTimeCodeFunction, getDword getRoundTimeFunction);
	static void update();

private:
	int _rows;
	int _columns;
	Size _frameSize;
	static int _defaultIndex;
	static int _timeCode;
	static DWORD _timer;
	static getInt _getTimeCode;
	static getDword _getRoundTime;

	RECT** _imageRectCache;
};

#endif