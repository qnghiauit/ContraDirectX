#include "Sprite.h"

vector<Sprite*> Sprite::_synchronousSpriteList;

// -----------------------------------------------
// Desc: Store new info to ready to load and use.
// -----------------------------------------------
Sprite::Sprite(const tchar* spritePath,
	int nColumns, int nRows, int nFrame,
	int currentFrame) : Texture(spritePath)
{
	this->_isComplete = true;
	this->_nColumns = nColumns;
	this->_nRows = nRows;
	this->_currentFrame = currentFrame;
	this->_fromLastSwitch = 0;
	this->_autoSwitchFrame = true;
	this->_switchFrameTime = DEFAULT_FRAME_TIME;

	if (nFrame > 0)
		this->_nFrame = nFrame;
	else
		this->_nFrame = _nRows * _nColumns;
}


// -----------------------------------------------
// Desc: Copy information from source sprite.
// -----------------------------------------------
Sprite::Sprite(const Sprite& sourceSprite) : Texture(sourceSprite)
{
	this->_isComplete = true;
	this->_currentFrame = 0;
	this->_nColumns = sourceSprite._nColumns;
	this->_nRows = sourceSprite._nRows;
	this->_nFrame = sourceSprite._nFrame;
	this->_fromLastSwitch = 0;
	this->_autoSwitchFrame = sourceSprite._autoSwitchFrame;
	this->_switchFrameTime = sourceSprite._switchFrameTime;

	if (this->isLoaded()) {
		this->_frameSize = sourceSprite._frameSize;
	}
}


// -----------------------------------------------
// Desc: Create a sprite from a texture.
// -----------------------------------------------
Sprite::Sprite(const Texture& sourceTexture,
	const int& nColumns, const int& nRows) : Texture(sourceTexture)
{
	this->_isComplete = true;
	this->_nColumns = nColumns;
	this->_nRows = nRows;
	this->_currentFrame = 0;
	this->_fromLastSwitch = 0;
	this->_autoSwitchFrame = true;
	this->_switchFrameTime = DEFAULT_FRAME_TIME;

	if (this->isLoaded())
		caculateFrameSize();
}


// -----------------------------------------------
// Desc: Create a sprite from read info.
// -----------------------------------------------
Sprite::Sprite(const tchar* path, fstream& infoFile)
{
	int columns;
	int rows;
	int totalFrames;

	infoFile >> columns;
	infoFile >> rows;
	infoFile >> totalFrames;

	this->Sprite::Sprite(path, columns, rows, totalFrames);
	this->_switchFrameTime = DEFAULT_FRAME_TIME;
	this->_autoSwitchFrame = true;
}

Sprite::~Sprite(void)
{
}


// -----------------------------------------------
// Desc: Base on num of Columns and Rows, this function will calculate frame size base on image resolution.
// -----------------------------------------------
void Sprite::caculateFrameSize()
{
	this->_frameSize.width = (int)((float)getInfo().Width / (float)this->_nColumns);
	this->_frameSize.height = (int)((float)getInfo().Height / (float)this->_nRows);
}


// -----------------------------------------------
// Desc: Override from T6_Texture, reset it to very first status.
// -----------------------------------------------
void Sprite::reset()
{
	this->setFrame(0);
	this->_isComplete = false;
	this->_fromLastSwitch = 0;
}


// -----------------------------------------------
// Desc: Load image and calculate frame size base on stored info.
// -----------------------------------------------
bool Sprite::loadSprite()
{
	Texture::loadImageFromStoredInfo();

	if (!this->isLoaded()) {
		WARNING_BOX(WARNING_CANNOT_LOAD_SPRITE);
		return false;
	}

	caculateFrameSize();
	return true;
}


// -----------------------------------------------
// Desc: Check and process if it's time to jump to next frame.
// -----------------------------------------------
bool Sprite::processToNextFrame(double passedGameTime)
{
	if (this->_fromLastSwitch < _switchFrameTime)
		_fromLastSwitch += passedGameTime;
	else {
		_currentFrame += _switchStep;
		_fromLastSwitch = 1;
	}

	if (_currentFrame == _currentRenderStopPoint) {
		_currentFrame = _currentRenderStartPoint;
		_fromLastSwitch = 0;
		this->_isComplete = true;
		return true;
	}
	else {
		this->_isComplete = false;
		return false;
	}
}


// -----------------------------------------------
// Desc: Draw frame-per-frame onto screen.
// -----------------------------------------------
bool Sprite::render(double passedGameTime, D3DXVECTOR3 position,
	float angle, D3DXVECTOR2 scale, D3DXVECTOR2 translate,
	DrawCenter drawCenter, D3DCOLOR color, int from, int to, RECT* sourceRect)
{
	if (!isLoaded())	{ loadSprite(); }

	Sprite::update();

	if (passedGameTime == 0)
	{
		from += _defaultIndex;
		to += _defaultIndex;
	}

	D3DXVECTOR3 center = computeCenter(drawCenter);

	return render(passedGameTime, position, angle, scale, translate, center, color, from, to, sourceRect);
}



bool Sprite::render(double passedGameTime /*= 0.0*/, 
						D3DXVECTOR3 position /*= D3DXVECTOR3(0, 0, 0.5)*/, 
						float visiblePercent_width,
						float visiblePercent_height,
						bool isTranslateXAxis,
						D3DXVECTOR2 scale,
						DrawCenter drawCenter /*= DrawCenter::CENTER*/, 
						int from /*= 0*/,
						int to /*= -1*/)
{
	if (!isLoaded())	{ loadSprite(); }

	Sprite::update();

	if (passedGameTime == 0)
	{
		from += _defaultIndex;
		to += _defaultIndex;
	}

	RECT* sourceRect = new RECT(getSrcRect());

	D3DXVECTOR3 center = computeCenter(drawCenter);

	convertVisiblePercent(visiblePercent_width, visiblePercent_height, *sourceRect, position, isTranslateXAxis, scale);

	bool result = render(passedGameTime, position, ANGLE_NO, scale, TRANSLATE_NO, center, COLOR_NO, from, to, sourceRect);
	SAFE_RELEASE(sourceRect);

	return result;
}


bool Sprite::render(double passedGameTime /*= 0.0*/, D3DXVECTOR3 position /*= D3DXVECTOR3(0, 0, 0.5)*/, float angle /*= ANGLE_NO*/, D3DXVECTOR2 scale /*= SCALE_NO*/, D3DXVECTOR2 translate /*= TRANSLATE_NO*/, D3DXVECTOR3 drawCenter /*= POSITION_ZERO*/, D3DCOLOR color /*= D3DCOLOR_XRGB(255, 255, 255)*/, int from /*= 0*/, int to /*= -1*/, RECT* sourceRect /*= nullptr*/)
{
	if (to < 0)
		to = this->_nFrame;

	if (from < 0)
		from = this->_nFrame - 1;

	if (to >= from)
	{
		_switchStep = 1;
	}
	else
	{
		_switchStep = -1;
		to--;
	}

	if (_fromLastSwitch == 0)
		_currentFrame = from;

	_currentRenderStartPoint = from;
	_currentRenderStopPoint = to;

	if (!isLoaded())
		loadSprite();

	if (sourceRect == nullptr)
	{
		RECT rect;
		rect = getSrcRect();
		this->drawImage(position, &rect, scale, translate, drawCenter, angle, color);
	}
	else
		this->drawImage(position, sourceRect, scale, translate, drawCenter, angle, color);

	if (_autoSwitchFrame)
		return processToNextFrame(passedGameTime);

	return false;
}



// -----------------------------------------------
// Desc: Jump the current frame to this point.
// -----------------------------------------------
void Sprite::setFrame(const int& currentFrame)
{
	_currentFrame = currentFrame;
}


// -----------------------------------------------
// Desc: Calculate and return the Rectangle define current draw range on texture.
// -----------------------------------------------
RECT Sprite::getSrcRect()
{
	RECT rect;
	rect.left = (UINT)_frameSize.width  * (_currentFrame - (int)(_currentFrame / _nColumns) * _nColumns);
	rect.top = (UINT)_frameSize.height * (int)(_currentFrame / _nColumns);
	rect.right = rect.left + (UINT)_frameSize.width;
	rect.bottom = rect.top + (UINT)_frameSize.height;
	return rect;
}


// -----------------------------------------------
// Desc: Return it's num of columns.
// -----------------------------------------------
int Sprite::getColumns()
{
	return _nColumns;
}



// -----------------------------------------------
// Desc: Override from T6_Texture's method, return the size of per frame.
// -----------------------------------------------
Size Sprite::getFrameSize()
{
	if (!isLoaded())
		loadSprite();

	if (_frameSize.x == 0 || _frameSize.y == 0)
	{
		caculateFrameSize();
	}

	return _frameSize;
}



// -----------------------------------------------
// Desc: Copy information from source Sprite.
// -----------------------------------------------
Sprite& Sprite::operator=(Sprite sourceSprite)
{
	memcpy(this, &sourceSprite, sizeof(sourceSprite));
	return *this;
}


// -----------------------------------------------
// Desc: Override from Texture, return the current frame.
// -----------------------------------------------
unsigned int Sprite::getCurrentFrame() const
{
	return _currentFrame;
}


// -----------------------------------------------
// Desc: Override from Texture, return the num of frames.
// -----------------------------------------------
unsigned int Sprite::getTotalFrames() const
{
	return _nFrame;
}


bool Sprite::isComplete()
{
	return (_fromLastSwitch == 0);
}


// -----------------------------------------------
// Desc: To set this sprite's auto-switch-frame feature on or off.
// -----------------------------------------------
void Sprite::setAutoSwitchFrame(bool isOn)
{
	_autoSwitchFrame = isOn;

	if (!isOn) {
		FOR(_synchronousSpriteList.size())
			if (_synchronousSpriteList.at(i) == this)
				return;

		_synchronousSpriteList.push_back(this);
	}
}


// -----------------------------------------------
// Desc: To process the sprite which is set to unable to process frame automatically.
// -----------------------------------------------
void Sprite::synchronousProcessToNextFrame(double passedGameTime)
{
	FOR(_synchronousSpriteList.size())
		_synchronousSpriteList.at(i)->processToNextFrame(passedGameTime);
}


void Sprite::setFrameTime(double newTime)
{
	_switchFrameTime = newTime;
}


double Sprite::getFrameTime()
{
	return _switchFrameTime;
}


Sprite* Sprite::clone() const
{
	return new Sprite(*this);
}


void Sprite::update()
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


void Sprite::initialize(getInt getTimeCodeFunction, getDword getRoundTimeFunction)
{
	_getTimeCode = getTimeCodeFunction;
	_getRoundTime = getRoundTimeFunction;
	_defaultIndex = 0;
	_timeCode = 0;
	_timer = 0;
}

getDword Sprite::_getRoundTime;

getInt Sprite::_getTimeCode;

DWORD Sprite::_timer;

int Sprite::_timeCode;

int Sprite::_defaultIndex;
