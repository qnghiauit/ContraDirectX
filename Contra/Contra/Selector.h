#ifndef __SELECTOR_H__
#define __SELECTOR_H__

#include "GameDefaultConstant.h"
#include "FreeText.h"
#include "Timer.h"

#include "DX9Framework/Texture.h"
#include "DX9Framework/Text.h"

#include <vector>

#define SELECTOR_RESOURCE_LINK	(tstring(_T("Resource//Miscellaneous//Selector")) + EXTENSION_PICTURE)
#define SELECTOR_RESOURCE_ROWS	1
#define SELECTOR_RESOURCE_COLUMNS	1
#define SELECTOR_DISTANCE_TO_SELECTOR_CENTER	D3DXVECTOR3(-12.0F, -4.0F, 0.0F)
#define SELECTOR_DISTANCE_BETWEEN_SELECTION	D3DXVECTOR3(0.0F, -16.0F, 0.0F)
#define SELECTOR_FLASH_TIME	300

enum SelectorDrawType
{
	DRAW_ALL = 0,
	FLASH_SELECTED,
	NUM_OF_DRAW_TYPE
};

class Selector
{
public:
	Selector();
	Selector(D3DXVECTOR3 position);
	~Selector();

	void assignSelection(const tchar* selectionName);

	void toNextSelection();

	void toPrevSelection();

	void setSelection(int index);

	const tchar* select();

	const tchar* getSelectionName();

	int getSelectionIndex();

	void draw(bool isDrawSelectorSign);

	void reset();

private:
	static Texture* _texture;
	D3DXVECTOR3 _position;
	int _currentSelection;
	vector<const tchar*> _selection;
	DWORD _timer;
	bool _isSelectedDraw;
	SelectorDrawType _drawType;
};

#endif // Selector_h__
