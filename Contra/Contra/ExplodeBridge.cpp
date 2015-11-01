#include "ExplodeBridge.h"


Pretties** ExplodeBridge::_destroyedBridgePretties = nullptr;

ExplodeBridge::ExplodeBridge(Object* baseObject, D3DXVECTOR3 position, RECT boundRange, ExplodeBridgeOrder order)
	: Pretties(EXPLODE_BRIDGE_RESOURCE_LINK, 0,
	EXPLODE_BRIDGE_RESOURCE_ROWS,
	EXPLODE_BRIDGE_FRAME_NUM, position,
	RESOURCE_MS)
	, AnimatedPretties(EXPLODE_BRIDGE_RESOURCE_LINK, 0,
	EXPLODE_BRIDGE_RESOURCE_ROWS, position,
	EXPLODE_BRIDGE_FRAME_NUM)
	, Tile(boundRange.left, boundRange.right, boundRange.top, boundRange.bottom,
	0/*==CharacterStatus::ON_GROUND*/, TileType::TOP_ONLY)
{
	if (_destroyedBridgePretties == nullptr)
		ExplodeBridge::initializeBridgeStaticResource();

	_position = position;
	this->_order = order;
	this->_baseObjectForUpdate = baseObject;
	this->_currentBridgeStatus = ExplodeBrideStatus::BRIDGE_WAIT;
	this->_destroyTimeRemaining = 0;
}


ExplodeBridge::~ExplodeBridge()
{
}


void ExplodeBridge::initializeBridgeStaticResource()
{
	_destroyedBridgePretties = new Pretties*[Side::NUM_OF_SIDE];

	FOR_e(Side::NUM_OF_SIDE)
		_destroyedBridgePretties[i] = new Pretties(tstring(LINK_BRIDGE_DESTROYED) + EXTENSION_PICTURE,
		i + 1, 1, Side::NUM_OF_SIDE,
		WDPOS_TOP_LEFT, RESOURCE_MT, (DrawCenter)i);
}


ExplodeBridge* ExplodeBridge::createBridge(Object* baseObject, Tile* leftside_BrideHead, Tile* rightside_BrideHead,
	vector<Tile*>* tileList, ExplodeBridgeOrder order)
{
	D3DXVECTOR3 position(leftside_BrideHead->getRect().right,
		leftside_BrideHead->getRect().bottom, leftside_BrideHead->getPosition().z);

	RECT boundRange;
	boundRange.left = (LONG)position.x;
	boundRange.bottom = (LONG)position.y;
	boundRange.right = (LONG)boundRange.left + (LONG)EXPLODE_BRIDGE_LONG;
	boundRange.top = (LONG)boundRange.bottom + (LONG)EXPLODE_BRIDGE_THICKNESS;

	ExplodeBridge* bridgePart = new ExplodeBridge(baseObject, position, boundRange, ExplodeBridgeOrder::BRIDGE_MIDDLE);

	bridgePart->_bridgehead_LinkedTile = new Tile*[Side::NUM_OF_SIDE];
	bridgePart->_bridgehead_LinkedTile[Side::SIDE_LEFT] = leftside_BrideHead;

	if (bridgePart->getRect().right < rightside_BrideHead->getRect().left)
	{
		bridgePart->_bridgehead_LinkedTile[Side::SIDE_RIGHT] = ExplodeBridge::createBridge(nullptr, bridgePart, rightside_BrideHead,
			tileList, ExplodeBridgeOrder::BRIDGE_MIDDLE);
		bridgePart->_linkedPretties.push_back(new Pretties(EXPLODE_BRIDGE_RESOURCE_LINK,
			order + EXPLODE_BRIDGE_FRAME_NUM + 1,
			EXPLODE_BRIDGE_RESOURCE_ROWS,
			EXPLODE_BRIDGE_FRAME_NUM,
			bridgePart->getPosition(),
			RESOURCE_MT,
			DrawCenter::TOPLEFT));
	}
	else
	{
		bridgePart->_bridgehead_LinkedTile[Side::SIDE_RIGHT] = rightside_BrideHead;
		bridgePart->_order = ExplodeBridgeOrder::NUM_OF_ORDER;
		bridgePart->_linkedPretties.push_back(new Pretties(EXPLODE_BRIDGE_RESOURCE_LINK,
			ExplodeBridgeOrder::BRIDGEHEAD_RIGHT + EXPLODE_BRIDGE_FRAME_NUM + 1,
			EXPLODE_BRIDGE_RESOURCE_ROWS,
			EXPLODE_BRIDGE_FRAME_NUM,
			bridgePart->getPosition(),
			RESOURCE_MT,
			DrawCenter::TOPLEFT));
	}

	tileList->push_back(bridgePart);
	return bridgePart;
}


void ExplodeBridge::releaseExplodeBridgeResource()
{
	FOR_e(Side::NUM_OF_SIDE)	SAFE_RELEASE(_destroyedBridgePretties[i]);

	SAFE_RELEASE(_destroyedBridgePretties);
}


void ExplodeBridge::draw()
{
	if (_currentBridgeStatus < ExplodeBrideStatus::BRIDGE_DETONATE)
	{
		Tile::draw();
		AnimatedPretties::draw();
	}
}


void ExplodeBridge::update()
{
	switch (_currentBridgeStatus)
	{
	case ExplodeBrideStatus::BRIDGE_WAIT:
		if (_baseObjectForUpdate != nullptr)
			if (_baseObjectForUpdate->getPosition().x >= _position.x)
				detonate();
		break;

	case ExplodeBrideStatus::BRIDGE_EXPLODE:
		if (_destroyTimeRemaining <= Timer::getRoundTime())
		{
			_currentBridgeStatus = ExplodeBrideStatus::BRIDGE_DETONATE;
			deactivate(PROPERTIES_VISIBLE);
			D3DXVECTOR3 leftPos(_position);
			D3DXVECTOR3 rightPos(_bridgehead_LinkedTile[Side::SIDE_RIGHT]->getRect().left, 
								_bridgehead_LinkedTile[Side::SIDE_RIGHT]->getRect().bottom,
								_bridgehead_LinkedTile[Side::SIDE_RIGHT]->getPosition().z);

			leftPos.y += (float)(*_sprites)->getFrameSize().height;
			rightPos.y += (float)(*_sprites)->getFrameSize().height;

			_bridgehead_LinkedTile[Side::SIDE_LEFT]->pushAPretties(ExplodeBridge::_destroyedBridgePretties[SIDE_LEFT]->clone(leftPos));
			_bridgehead_LinkedTile[Side::SIDE_RIGHT]->pushAPretties(ExplodeBridge::_destroyedBridgePretties[SIDE_RIGHT]->clone(rightPos));
			_destroyTimeRemaining = (DWORD)(EffectManager::getExplodeTime() * (EXPLODE_NUM_FOR_DESTROY_ANIMATION + EXPLODE_BRIDGE_FRAME_NUM));
		}
		else
			_destroyTimeRemaining -= Timer::getRoundTime();
		break;

	case ExplodeBrideStatus::BRIDGE_DETONATE:
		if (_destroyTimeRemaining <= Timer::getRoundTime())
		{
			_bridgehead_LinkedTile[Side::SIDE_RIGHT]->activate();
			_currentBridgeStatus = ExplodeBrideStatus::BRIDGE_DISSAPEAR;
		}
		else
			_destroyTimeRemaining -= Timer::getRoundTime();
		break;

	case ExplodeBrideStatus::BRIDGE_DISSAPEAR:
		break;
	default:
		break;
	}
}


void ExplodeBridge::detonate()
{
	_currentBridgeStatus = ExplodeBrideStatus::BRIDGE_EXPLODE;
	srand((unsigned int)time(0));
	FOR(EXPLODE_NUM_FOR_DESTROY_ANIMATION)
		EffectManager::addExplode(D3DXVECTOR3(_position.x + EXPLODE_BRIDGE_RANDOM_IN_WIDTH,
		_position.y + EXPLODE_BRIDGE_RANDOM_IN_HEIGHT,
		DEFAULT_DEPTH), (DWORD)(i * EffectManager::getExplodeTime()));

	_destroyTimeRemaining = (DWORD)(EffectManager::getExplodeTime());
}


void ExplodeBridge::activate()
{
	detonate();
}

bool ExplodeBridge::affect(Object* obj, DWORD key)
{
	Tile::affect(obj, key);
	_bridgehead_LinkedTile[Side::SIDE_RIGHT]->affect(obj, key);
	return false;
}

bool ExplodeBridge::isTerminated()
{
	return (_currentBridgeStatus = ExplodeBrideStatus::BRIDGE_DISSAPEAR);
}
