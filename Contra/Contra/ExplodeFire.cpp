#include "ExplodeFire.h"


ExplodeFire::ExplodeFire(D3DXVECTOR3 pos)
{
	_position = pos;
	setBasicProperties();
	*_objectDefRect = Default::createRectFromCenter(getPosition(), EXFIRE_BOUND_SIZE);
	Object::loadObjectResource(EXFIRE_RESOURCE_LINK.c_str(), EXFIRE_RESOURCE_ROWS,
								EXFIRE_RESOURCE_COLUMNS, RESOURCE_MS);
}


ExplodeFire::~ExplodeFire()
{
}

unsigned int ExplodeFire::getScore()
{
	return 0;
}

void ExplodeFire::draw()
{
	(*_sprites)->drawImage(getPosition(), nullptr, _scale, TRANSLATE_NO, DrawCenter::CENTER);
}

void ExplodeFire::waitUpdate()
{
	activate();
}

void ExplodeFire::activateUpdate()
{
	if (_currentCollisEvents.size() > 0)
		damaged(BULLET_DAMAGE_MASSIVE);
}

void ExplodeFire::deadUpdate()
{
	// Do nothing
}

bool ExplodeFire::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

void ExplodeFire::setBasicProperties()
{
	Mobs::setBasicProperties();
	MoveObject::setBasicProperties();
}

void ExplodeFire::deadEffect()
{
	EffectManager::addExplode(Default::computeCenter(getRect()));
}

bool ExplodeFire::isTerminated()
{
	return Mobs::isDead();
}

void ExplodeFire::update()
{
	Mobs::update();

	MoveObject::updateMoveableWithCollision(EXFIRE_BOUND_SIZE, true, 0, Default::createRectFromCenter);

	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);
}
