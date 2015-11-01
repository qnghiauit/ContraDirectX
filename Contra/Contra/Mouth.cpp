#include "Mouth.h"

Mouth::Mouth(D3DXVECTOR3 pos, Object* player)
{
	setBasicProperties();
	_position = pos + MOUTH_MOUTH_TRANSLATE;
	basePos_ = pos;
	_target = player;
	*_objectDefRect = Default::createRectFromDrawCenter(getPosition(), MOUTH_BOUND_SIZE, DrawCenter::TOPCENTER);
	Object::loadObjectResource(MOUTH_RESOURCE_MOUTH.c_str(), MOUTH_RESOURCE_MOUTH_ROWS, MOUTH_RESOURCE_MOUTH_COLUMNS, RESOURCE_MS);

	assignSubPart(new Tentacle(getPosition() + MOUTH_TENTACLE_L_POS_FIX, MOUTH_TENTACLE_L_ANGLE, player));
	assignSubPart(new Tentacle(getPosition() + MOUTH_TENTACLE_R_POS_FIX, MOUTH_TENTACLE_R_ANGLE, player));
}


Mouth::~Mouth()
{
	SAFE_RELEASE(_staticBase);
	SAFE_RELEASE(_livingBase);
	SAFE_RELEASE(_head);
}

unsigned int Mouth::getScore()
{
	return Boss::getScore();
}

void Mouth::draw()
{
	static int timer = 0;

	_staticBase->drawImage(basePos_, nullptr, SCALE_NO, MOUTH_STATIC_BASE_TRANSLATE, DrawCenter::BOTTOMCENTER, ANGLE_NO, _opacity);

	if (!isDead())
	{
		timer += Timer::getRoundTime();
		if (timer >= DEFAULT_FRAME_TIME)
		{
			_currentEyeColorIndex = (_currentEyeColorIndex + 1) % MOUTH_EYES_NUM_OF_COLOS;
			timer = 0;
		}

		static MOUTH_EYES_COLORS;
		_livingBase->drawImage(basePos_, nullptr, SCALE_NO, MOUTH_LIVING_BASE_TRANSLATE, DrawCenter::BOTTOMCENTER, ANGLE_NO, _opacity);
		_head->draw(0, getPosition(), DrawCenter::TOPCENTER, SCALE_NO, MOUTH_HEAD_TRANSLATE, ANGLE_NO, _opacity);
		_head->draw(1, getPosition(), DrawCenter::TOPCENTER, SCALE_NO, MOUTH_HEAD_TRANSLATE, ANGLE_NO, eyeColors[_currentEyeColorIndex] & _opacity);

		switch (_status)
		{
		case MOUTH_CLOSE:
			_sprites[0]->render(Timer::getRoundTime(), getPosition(),
				ANGLE_NO, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPCENTER, _opacity, 0, 0);
			break;

		case MOUTH_CLOSING:
			_sprites[0]->render(Timer::getRoundTime(), getPosition(),
				ANGLE_NO, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPCENTER, _opacity, MOUTH_FRAME_NUM - 1, 0);
			break;

		case MOUTH_OPENING:
			_sprites[0]->render(Timer::getRoundTime(), getPosition(),
				ANGLE_NO, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPCENTER, _opacity, 0, -1);
			break;

		case MOUTH_OPEN:
			_sprites[0]->render(Timer::getRoundTime(), getPosition(),
				ANGLE_NO, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPCENTER, _opacity, MOUTH_FRAME_NUM - 1, MOUTH_FRAME_NUM - 1);
			break;
		default:
			break;
		}
	}

	Boss::draw();
}

void Mouth::waitUpdate()
{
	Boss::waitUpdate();
}

void Mouth::activateUpdate()
{
	Boss::activateUpdate();
	_timeCounter += Timer::getRoundTime();
	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);

	switch (_status)
	{
	case MOUTH_CLOSE:
		if (_timeCounter >= MOUTH_CLOSE_TIME)
		{
			_timeCounter = 0;
			_status = MOUTH_OPENING;
		}
		break;

	case MOUTH_CLOSING:
		if (_sprites[0]->isComplete())
		{
			_timeCounter = 0;
			_status = MOUTH_CLOSE;
			Object::activate(PROPERTIES_EVATION);
		}
		break;

	case MOUTH_OPENING:
		if (_sprites[0]->isComplete())
		{
			_timeCounter = 0;
			_status = MOUTH_OPEN;
			Object::deactivate(PROPERTIES_EVATION);
		}
		break;

	case MOUTH_OPEN:
		if (_timeCounter <= Timer::getRoundTime())
		{
			FOR(MOUTH_NUM_OF_SHOOT_ANGLE)
				instanceShoot();
		}
		
		if (_timeCounter >= MOUTH_OPEN_TIME)
		{
			_timeCounter = 0;
			_status = MOUTH_CLOSING;
		}
		break;
	default:
		break;
	}
}

void Mouth::deadEffect()
{
	Boss::deadEffect();
	srand((unsigned int)time(nullptr));
	FOR(10)
	{
		D3DXVECTOR3 effectPos;
		effectPos.x = _position.x + MOUTH_LIVING_BASE_TRANSLATE.x + (rand() % (int)_livingBase->getFrameSize().width);
		effectPos.y = _position.y + MOUTH_LIVING_BASE_TRANSLATE.y + (rand() % (int)_livingBase->getFrameSize().height);
		effectPos.z = DEFAULT_DEPTH;

		EffectManager::addExplode(
			effectPos,
			i * DEFAULT_FRAME_TIME);
	}
}

void Mouth::setBasicProperties()
{
	Boss::setBasicProperties();
	ShootObject::setBasicProperties();
	Object::activate(PROPERTIES_EVATION);

	_heathPoints = MOUTH_LIFE_POINTS;

	_currentBullet = BulletType::FIRE_CIRCLE;

	_staticBase = new Texture(MOUTH_STATIC_BASE.c_str());
	_livingBase = new Texture(MOUTH_LIVING_BASE.c_str());
	_head = new MergedTexture(MOUTH_RESOURCE_HEAD.c_str(), MOUTH_RESOURCE_HEAD_COLUMNS, MOUTH_RESOURCE_HEAD_ROWS);
	_opacity = 0x00ffffff;
}

void Mouth::update()
{
	Boss::update();
}

int Mouth::computeShootAngle()
{
	static int angleIndex = 0;

	static MOUTH_SHOOT_ANGLES;

	angleIndex = (angleIndex + 1) % MOUTH_NUM_OF_SHOOT_ANGLE;
	return shootAngles[angleIndex];
}

D3DXVECTOR3 Mouth::computeShootPoint()
{
	return getPosition();
}

bool Mouth::affect(Object* obj, DWORD key)
{
	return false;
}

bool Mouth::isTerminated()
{
	return Boss::isTerminated();
}

void Mouth::activate()
{
	DWORD fade = (DWORD)TRUE_SPEED(MOUTH_FADE_SPEED, Timer::getRoundTime());
	
	if ((_opacity >> 24) > (255 - fade))
	{
		_opacity = 0xffffffff;
		Boss::activate();
	}
	else
	{
		_opacity += (fade << 24);
	}
}