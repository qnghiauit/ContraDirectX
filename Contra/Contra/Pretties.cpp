#include "Pretties.h"

Pretties::Pretties(tstring path, index_t index, size_t rows, size_t columns, D3DXVECTOR3 position,
					int resourceType, DrawCenter drawCenter)
{
	Object::setBasicProperties();
	_currentImmortalStatus = Object::propertiesYes;
	_currentShootableStatus = Object::propertiesNo;
	_currentTouchableStatus = Object::propertiesNo;

	Object::loadObjectResource(path.c_str(), rows, columns, resourceType);

	_index = index;
	_position = position;

	this->_drawCenter = drawCenter;

	*this->_objectDefRect = Default::createRectFromDrawCenter(_position, (*_sprites)->getFrameSize(), _drawCenter);
}


Pretties::Pretties(const Pretties& p)
{
	this->_index = p._index;
	this->_drawCenter = p._drawCenter;
	this->_sprites = new Texture*();
	this->_objectDefRect = new RECT(*p._objectDefRect);
	*_sprites = p._sprites[0]->clone();
}


Pretties::~Pretties()
{
}


void Pretties::draw() const
{
	if (_index > 0)
		((MergedTexture*)(*_sprites))->draw(_index - 1, _position, _drawCenter);
}


void Pretties::draw()
{
	const Pretties* drawObject = this;
	drawObject->draw();
}


void Pretties::update()
{

}


Pretties* Pretties::clone() const
{
	return new Pretties(*this);
}


Pretties* Pretties::clone(D3DXVECTOR3 newPos) const
{
	Pretties* result = this->clone();
	result->_position = newPos;
	*result->_objectDefRect = Default::createRectFromDrawCenter(result->_position, (*_sprites)->getFrameSize(), _drawCenter);
	return result;
}

bool Pretties::affect(Object* obj, DWORD key)
{
	return false;
}

bool Pretties::isTerminated()
{
	return false;
}
