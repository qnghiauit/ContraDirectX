#include "Texture.h"

LPD3DXSPRITE		Texture::_handler	= 0;
D3DXMATRIX			Texture::_cameraConvertMatrix;
LPDIRECT3DDEVICE9	Texture::_d3dDevice	= 0;
vector<Texture*>	Texture::_loadedTexture;

// -----------------------------------------------
// Desc: Initialize basic properties to use this class.
// -----------------------------------------------
HRESULT Texture::initializeHandler(const LPD3DXSPRITE& handler)
{
	HRESULT result;

	Texture::_handler = handler;
	result = (_handler == 0);
	result = result | Texture::_handler->GetDevice (&_d3dDevice);

	return result;
}


// -----------------------------------------------
// Desc: Release it's properties after use (close Game).
// -----------------------------------------------
HRESULT Texture::releaseHandler()
{
	if (_d3dDevice != 0)
		return _d3dDevice->Release();

	FOR(_loadedTexture.size())
	{
		_loadedTexture.at(i)->releaseTexture();
		SAFE_RELEASE(_loadedTexture.at(i));
	}
	_loadedTexture.clear();

	return S_OK;
}


// -----------------------------------------------
// Desc: Get the basic info, ready to load.
// -----------------------------------------------
Texture::Texture(const tchar* path)
{
	_path = new tstring(path);
	this->_info = nullptr;
}


// -----------------------------------------------
// Desc: Copy the info from another one.
// -----------------------------------------------
Texture::Texture(const Texture& newTexture)
{
	this->_path		= new tstring(newTexture._path->c_str());

	if (newTexture.isLoaded()) {
		this->_info		= newTexture._info;
		this->_texture = newTexture._texture;
	}
}


// -----------------------------------------------
// Desc: Wipe out information to destroy.
// -----------------------------------------------
Texture::~Texture(void)
{
	SAFE_RELEASE(_path);
	_info = nullptr;
	_texture = nullptr;
}


// -----------------------------------------------
// Desc: Return handler handling this texture,
// -----------------------------------------------
LPD3DXSPRITE Texture::getHandler() const
{
	return Texture::_handler;
}


// -----------------------------------------------
// Desc: Return it's path.
// -----------------------------------------------
const tchar* Texture::getPath() const
{
	return this->_path->c_str();
}


// -----------------------------------------------
// Desc: Return the loaded image info.
// -----------------------------------------------
D3DXIMAGE_INFO Texture::getInfo() const
{
	return *this->_info;
}


// -----------------------------------------------
// Desc: Return the loaded image frame size (override by T6_Sprite).
// -----------------------------------------------
Size Texture::getFrameSize()
{
	if (!isLoaded())
		loadImageFromStoredInfo();

	return Size(this->_info->Width, this->_info->Height);
}


// -----------------------------------------------
// Desc: Set texture to it very first status.
// -----------------------------------------------
void Texture::reset()
{
	
}


// -----------------------------------------------
// Desc: Load image stored in path_.
// -----------------------------------------------
void Texture::loadImageFromStoredInfo()
{
	Texture* cacheTexture = tryLoadFromCache();

	if (cacheTexture != 0) {
		_info = cacheTexture->_info;
		_texture = cacheTexture->_texture;
		cacheTexture = 0;
	}
	else {
		_texture = new LPDIRECT3DTEXTURE9();
		_info = new D3DXIMAGE_INFO();
		HRESULT result = D3DXGetImageInfoFromFile(_path->c_str(), _info);

		if (FAILED(result)) {
			WARNING_BOX((*_path + WARNING_CANNOT_LOAD_IMAGE).c_str());
			return;
		}

		result = D3DXCreateTextureFromFileEx(_d3dDevice,
											_path->c_str(),
											_info->Width,
											_info->Height,
											D3DX_FROM_FILE, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN,
											D3DPOOL_DEFAULT, D3DX_DEFAULT,
											D3DX_DEFAULT,
											0, _info, 0, 
											_texture);

		if (FAILED(result)) {
			WARNING_BOX((*_path + WARNING_CANNOT_LOAD_IMAGE).c_str());
		}
		else {
			_loadedTexture.push_back(new Texture(*this));
		}
	}
}


// -----------------------------------------------
// Desc: Try to find if imaged is loaded before.
// -----------------------------------------------
Texture* Texture::tryLoadFromCache()
{
	FOR (_loadedTexture.size())
	{
		try
		{
			if (*_path == *(_loadedTexture.at(i)->_path))
				return _loadedTexture.at(i);
		}
		catch (exception e)
		{
			_loadedTexture.erase(_loadedTexture.begin() + i);
			i--;
		}
	}

	return 0;
}


// -----------------------------------------------
// Desc: Check if the texture is loaded.
// -----------------------------------------------
bool Texture::isLoaded() const
{
	return (_texture != 0 && _info != nullptr);
}



// -----------------------------------------------
// Desc: Return the center point of the "current frame" base on draw center (the center point in the human word).
//		 Computing on machine axis(es).
// -----------------------------------------------
D3DXVECTOR3 Texture::computeCenter(const DrawCenter& drawCenter)
{
		 if (drawCenter == DrawCenter::TOPLEFT		)	return D3DXVECTOR3(0.0F,								0.0F,									DEFAULT_DEPTH);
	else if (drawCenter == DrawCenter::BOTTOMLEFT	)	return D3DXVECTOR3(0.0F,								(float)getFrameSize().height,			DEFAULT_DEPTH);
	else if (drawCenter == DrawCenter::TOPCENTER	)	return D3DXVECTOR3((float)getFrameSize().width / 2.0F,	0.0F,									DEFAULT_DEPTH);
	else if (drawCenter == DrawCenter::TOPRIGHT		)	return D3DXVECTOR3((float)getFrameSize().width,			0.0F,									DEFAULT_DEPTH);
	else if (drawCenter == DrawCenter::CENTER		)	return D3DXVECTOR3((float)getFrameSize().width / 2.0F,	(float)getFrameSize().height / 2.0F,	DEFAULT_DEPTH);
	else if (drawCenter == DrawCenter::BOTTOMCENTER	)	return D3DXVECTOR3((float)getFrameSize().width / 2.0F,	(float)getFrameSize().height,			DEFAULT_DEPTH);
	else if (drawCenter == DrawCenter::BOTTOMRIGHT	)	return D3DXVECTOR3((float)getFrameSize().width,			(float)getFrameSize().height,			DEFAULT_DEPTH);
	else return WDPOS_TOP_LEFT;
}


// -----------------------------------------------
// Desc: Scale, rotate and translate image follow passed variables.
// -----------------------------------------------
void Texture::transformImage(const float& angle, const D3DXVECTOR2& scale, const D3DXVECTOR2& translate,
								const D3DXVECTOR3& drawCenter, const D3DMATRIX& originalStatus)
{
	if(angle != ANGLE_NO || scale != SCALE_NO || translate != TRANSLATE_NO)
	{
		D3DXVECTOR2 center	= (D3DXVECTOR2)drawCenter;
		D3DXVECTOR2 tran	= D3DXVECTOR2(0,0);

		D3DXMATRIX newStatus;
		D3DXMatrixTransformation2D (&newStatus, &center, 0, &scale,
			&center,D3DXToRadian(angle), &translate);

		// compute and transform the image to current status.
		D3DXMATRIX finalMt = newStatus * originalStatus;
		this->_handler->SetTransform(&finalMt);
	}
}


// -----------------------------------------------
// Desc: Draw the defined range of image onto screen.
// -----------------------------------------------
void Texture::drawImage(D3DXVECTOR3 pos,RECT* srcRect, D3DXVECTOR2 scale, D3DXVECTOR2 translate,
						   const DrawCenter& drawCenter, float angle, D3DCOLOR color, bool converted)
{
	if (_texture == 0)	{ loadImageFromStoredInfo(); }

	D3DXVECTOR3 center = computeCenter(drawCenter);

	drawImage(pos, srcRect, scale, translate, center, angle, color, converted);
}


void Texture::drawImage(D3DXVECTOR3 pos /*= D3DXVECTOR3(0, 0, 0.5)*/, RECT* srcRect /*= 0*/, D3DXVECTOR2 scale /*= SCALE_NO*/, D3DXVECTOR2 translate /*= TRANSLATE_NO*/, D3DXVECTOR3 drawCenter /*= POSITION_ZERO*/, float angle /*= ANGLE_NO*/, D3DCOLOR color /*= 0xFFFFFFFF*/, bool converted /*= true*/)
{
	if (_texture == 0)	{ loadImageFromStoredInfo(); }

	if (converted) {
		D3DXVec3TransformCoord(&pos, &pos, &_cameraConvertMatrix);
	}

	// get image original status.
	D3DXMATRIX oldMt, newMt;
	this->_handler->GetTransform(&oldMt);
	this->_handler->GetTransform(&newMt);

	transformImage(angle, scale, translate, pos, newMt);

	pos.x = (float)(int)pos.x;
	pos.y = (float)(int)pos.y;

	_handler->Draw(*_texture, srcRect, &drawCenter, &pos, color);
	// set image back to original status.
	_handler->SetTransform(&oldMt);
}


// -----------------------------------------------
// Desc: Draw frame-per-frame onto screen, if it's texture, just draw it. (override by T6_Sprite)
// -----------------------------------------------
bool Texture::render(double passedGameTime, D3DXVECTOR3 position,
	float angle, D3DXVECTOR2 scale, D3DXVECTOR2 translate,
	DrawCenter drawCenter, D3DCOLOR color, int from, int to,
	RECT* sourceRect)
{
	if (!isLoaded())
		loadImageFromStoredInfo();

	this->drawImage(position, sourceRect, scale, translate, drawCenter, angle, color);

	return true;
}


bool Texture::render(double passedGameTime /*= 0.0*/, D3DXVECTOR3 position /*= D3DXVECTOR3(0, 0, 0.5)*/, float visiblePercent_width /*= 1.0f*/, float visiblePercent_height /*= 1.0f*/,
						bool isTranslateXAxis,
						D3DXVECTOR2 scale, DrawCenter drawCenter /*= DrawCenter::CENTER*/, int from /*= 0*/, int to /*= -1*/)
{
	if (!isLoaded())
		loadImageFromStoredInfo();

	RECT sourceRect;
	sourceRect.top = 0;
	sourceRect.left = 0;
	sourceRect.bottom = getFrameSize().height;
	sourceRect.right = getFrameSize().width;

	D3DXVECTOR3 center = computeCenter(drawCenter);

	convertVisiblePercent(visiblePercent_width, visiblePercent_height, sourceRect, position, isTranslateXAxis, scale);

	drawImage(position, &sourceRect, scale, TRANSLATE_NO, center, ANGLE_NO);

	return true;
}



// -----------------------------------------------
// Desc: Copy the information from source.
// -----------------------------------------------
Texture& Texture::operator=(const Texture& sourceTexture)
{
	memcpy(this, &sourceTexture, sizeof(sourceTexture));
	return *this;
}


// -----------------------------------------------
// Desc: Return the current frame, 0 if it's texture.
// -----------------------------------------------
unsigned int Texture::getCurrentFrame() const
{
	return 0;
}


// -----------------------------------------------
// Desc: Return the num of frames, 1 if it's texture.
// -----------------------------------------------
unsigned int Texture::getTotalFrames() const
{
	return 1;
}


// -----------------------------------------------
// Desc: Set new matrix to convert draw position.
// -----------------------------------------------
void Texture::setPositioncConvertMatrix(const D3DXMATRIX& newMatrix)
{
	Texture::_cameraConvertMatrix = newMatrix;
}


bool Texture::isComplete()
{
	return true;
}


void Texture::releaseTexture()
{
	SAFE_RELEASE(_info);
	SAFE_RELEASE(_texture);
}


Texture* Texture::clone() const
{
	return new Texture(*this);
}


void Texture::convertVisiblePercent(float width, float height, RECT& sourceRECT, D3DXVECTOR3& position, bool xAxisTranslate, D3DXVECTOR2 scale)
{
	float dWidth = sourceRECT.right - sourceRECT.left;
	float dHeight = sourceRECT.top - sourceRECT.bottom;

	if (xAxisTranslate)
		position.x += dWidth * (1 - width);
	position.y += dHeight * (1 - height);

	sourceRECT.bottom = sourceRECT.top - dHeight * height;

	if (scale.x >= 0)
		sourceRECT.right = sourceRECT.left + dWidth * width;
	else
	{
		sourceRECT.left = sourceRECT.right - dWidth * width;
	}
}