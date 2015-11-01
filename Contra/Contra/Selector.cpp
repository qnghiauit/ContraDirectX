#include "Selector.h"


Texture* Selector::_texture = nullptr;


Selector::Selector()
{
}


Selector::Selector(D3DXVECTOR3 position)
{
	_position = position;
	_currentSelection = 0;
	_drawType = DRAW_ALL;
	_isSelectedDraw = true;
	if (_texture == nullptr)
	{
		_texture = new Texture(SELECTOR_RESOURCE_LINK.c_str());
	}
}


Selector::~Selector()
{
	SAFE_RELEASE(_texture);
	_selection.clear();
}


void Selector::assignSelection(const tchar* selectionName)
{
	_selection.push_back(selectionName);
}

void Selector::toNextSelection()
{
	_currentSelection = (_currentSelection + 1) % _selection.size();
}

void Selector::toPrevSelection()
{
	_currentSelection--;
	if (_currentSelection < 0)
	{
		_currentSelection = _selection.size() - 1;
	}
}

const tchar* Selector::getSelectionName()
{
	return _selection.at(_currentSelection);
}

int Selector::getSelectionIndex()
{
	return _currentSelection;
}

void Selector::draw(bool isDrawSelectorSign)
{
	D3DXVECTOR3 tempPos = _position;

	_timer += Timer::getRoundTime();
	if (_timer >= SELECTOR_FLASH_TIME)
	{
		_timer = 0;
		if (_drawType == FLASH_SELECTED)
			_isSelectedDraw = !_isSelectedDraw;

	}

	FOR(_selection.size())
	{
		if (_currentSelection == i)
		{
			if (isDrawSelectorSign)
				_texture->drawImage(tempPos + SELECTOR_DISTANCE_TO_SELECTOR_CENTER, nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::CENTER);

			if (!_isSelectedDraw)
			{
				tempPos += SELECTOR_DISTANCE_BETWEEN_SELECTION;
				continue;
			}
		}

		FreeText::get().drawText(_selection.at(i), tempPos);

		tempPos += SELECTOR_DISTANCE_BETWEEN_SELECTION;
	}
}


const tchar* Selector::select()
{
	_drawType = FLASH_SELECTED;
	return getSelectionName();
}


void Selector::setSelection(int index)
{
	_currentSelection = index;
}

void Selector::reset()
{
	_currentSelection = 0;
	_drawType = DRAW_ALL;
	_isSelectedDraw = true;
}
