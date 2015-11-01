#include "MergedTexture.h"

MergedTexture::MergedTexture(const tchar* path, int nColumns, int nRows) : Texture(path)
{
	this->_rows = nRows;
	this->_columns = nColumns;
	
	_imageRectCache = new RECT*[(_rows * _columns)];
	FOR((_rows * _columns))
		_imageRectCache[i] = nullptr;
}


MergedTexture::MergedTexture(const MergedTexture& sourceMTexture) : Texture(sourceMTexture)
{
	this->_rows = sourceMTexture._rows;
	this->_columns = sourceMTexture._columns;

	_imageRectCache = new RECT*[(_rows * _columns)];
	this->_frameSize = sourceMTexture._frameSize;
	FOR((_rows * _columns))
		_imageRectCache[i] = sourceMTexture._imageRectCache[i];
}


MergedTexture::~MergedTexture()
{
	FOR(_rows * _columns)
		SAFE_RELEASE(_imageRectCache[i]);

	SAFE_RELEASE(_imageRectCache);
}


void MergedTexture::loadMergedTexture()
{
	Texture::loadImageFromStoredInfo();

	this->_frameSize.width = this->getInfo().Width / _columns;
	this->_frameSize.height = this->getInfo().Height / _rows;
}


void MergedTexture::draw(index_t index, D3DXVECTOR3 position, DrawCenter drawCenter,
							D3DXVECTOR2 scale, D3DXVECTOR2 translate, float angle, D3DCOLOR fadedColor)
{
	if (!isLoaded()) {
		loadMergedTexture();
	}

	if (index == DEFAULT_INDEX)
	{
		index = _defaultIndex;
	}

	MergedTexture::update();

	index_t indexrow = (index_t)(index / _columns);
	index_t indexcol = (index_t)(index % _columns);

	if (_imageRectCache[index] == nullptr) {
		_imageRectCache[index]			= new RECT();
		_imageRectCache[index]->left	= (LONG)(indexcol * _frameSize.width);
		_imageRectCache[index]->right	= (LONG)(_imageRectCache[index]->left + _frameSize.width);
		_imageRectCache[index]->top		= (LONG)(indexrow * _frameSize.height);
		_imageRectCache[index]->bottom	= (LONG)(_imageRectCache[index]->top + _frameSize.height);
	}

	drawImage(position, _imageRectCache[index], scale, translate, drawCenter, angle, fadedColor);
}

void MergedTexture::draw(index_t index, D3DXVECTOR3 position, float percentOfVisible_width, float percentOfVisible_height, bool	isTranslateXAxis,
	DrawCenter drawCenter /*= DrawCenter::TOPLEFT*/, D3DXVECTOR2 scale /*= SCALE_NO*/, D3DXVECTOR2 translate /*= TRANSLATE_NO*/, float angle /*= ANGLE_NO*/, D3DCOLOR fadedColor /*= 0xFFFFFFFF*/)
{
	if (!isLoaded()) {
		loadMergedTexture();
	}

	MergedTexture::update();

	if (index == DEFAULT_INDEX)
	{
		index = _defaultIndex;
	}

	index_t indexrow = (index_t)(index / _columns);
	index_t indexcol = (index_t)(index % _columns);

	if (_imageRectCache[index] == 0) {
		_imageRectCache[index] = new RECT();
		_imageRectCache[index]->left = (LONG)(indexcol * _frameSize.width);
		_imageRectCache[index]->right = (LONG)(_imageRectCache[index]->left + _frameSize.width);
		_imageRectCache[index]->top = (LONG)(indexrow * _frameSize.height);
		_imageRectCache[index]->bottom = (LONG)(_imageRectCache[index]->top + _frameSize.height);
	}

	RECT drawRect(*_imageRectCache[index]);
	convertVisiblePercent(percentOfVisible_width, percentOfVisible_height, drawRect, position, isTranslateXAxis, scale);

	drawImage(position, &drawRect, scale, translate, drawCenter, angle, fadedColor);
}


Size MergedTexture::getFrameSize()
{
	if (!isLoaded())
		loadMergedTexture();

	return _frameSize;
}


MergedTexture* MergedTexture::clone() const
{
	return new MergedTexture(*this);
}


void MergedTexture::initialize(getInt getTimeCodeFunction, getDword getRoundTimeFunction)
{
	_getTimeCode = getTimeCodeFunction;
	_getRoundTime = getRoundTimeFunction;
	_defaultIndex = 0;
	_timeCode = 0;
	_timer = 0;
}


void MergedTexture::update()
{
	if (_getTimeCode() != _timeCode)
	{
		_timeCode = _getTimeCode();
		_timer += _getRoundTime();
		if (_timer >= DEFAULT_FRAME_TIME)
		{
			_defaultIndex++;
			if (_defaultIndex >= ((~0) >> 1))
			{
				_defaultIndex = 0;
			}
		}
	}
}

getDword MergedTexture::_getRoundTime;

getInt MergedTexture::_getTimeCode;

DWORD MergedTexture::_timer;

int MergedTexture::_timeCode;

int MergedTexture::_defaultIndex;
