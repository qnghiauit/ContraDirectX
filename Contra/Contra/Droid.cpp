#include "Droid.h"
#include "Map.h"

Droid::Droid(D3DXVECTOR3 pos)
{
	setBasicProperties();
	basePos_ = pos;
	_position = pos + DROID_START_POS_FIX;
	randomRegion_.bottom = basePos_.y + DROID_BOUND_SIZE.y;
	randomRegion_.top = RESOLUTION_HEIGHT - DROID_BOUND_SIZE.y;
	randomRegion_.left = basePos_.x - RESOLUTION_WIDTH + DROID_BOUND_SIZE.x;
	randomRegion_.right = basePos_.x - DROID_BASE_WIDTH - DROID_BOUND_SIZE.x;
	*_objectDefRect = Default::createRectFromBottomCenter(getPosition(), DROID_BOUND_SIZE);
	Object::loadObjectResource(DROID_BASE_RESOURCE_LINK.c_str(), DROID_BASE_RESOURCE_ROWS,
								DROID_BASE_RESOURCE_COLUMNS, RESOURCE_MS);

	fire_ = new Sprite(DROID_FIRE_RESOURCE_LINK.c_str(), DROID_FIRE_RESOURCE_COLUMNS,
		DROID_FIRE_RESOURCE_ROWS);

	door_ = new Sprite(DROID_DOOR_RESOURCE_LINK.c_str(), DROID_DOOR_RESOURCE_COLUMNS,
		DROID_DOOR_RESOURCE_ROWS);

	staticBase_ = new Texture(DROID_STATIC_BASE.c_str());
	livingBase_ = new Texture(DROID_LIVING_BASE.c_str());
}


Droid::~Droid()
{
	SAFE_RELEASE(fire_);
	SAFE_RELEASE(door_);
	SAFE_RELEASE(staticBase_);
	SAFE_RELEASE(livingBase_);

	generatedObjects_.clear();
}

void Droid::draw()
{
	staticBase_->drawImage(basePos_, nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::BOTTOMRIGHT);

	if (!isDead())
	{
		livingBase_->drawImage(basePos_, nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::BOTTOMRIGHT);
		(*_sprites)->drawImage(getPosition(), nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::BOTTOMCENTER, ANGLE_NO, fadeColor_);
		fire_->render(Timer::getRoundTime(), getPosition(), ANGLE_NO, SCALE_NO, DROID_FIRE_TRANSLATE, DrawCenter::TOPCENTER, fadeColor_);
		
		switch (state_)
		{
		case DROID_APPEAR:
			door_->render(Timer::getRoundTime(), getPosition(), ANGLE_NO, SCALE_NO, DROID_DOOR_TRANSLATE,
				DrawCenter::BOTTOMCENTER, fadeColor_, -1, -1);
			break;
		case DROID_DISSAPEAR:
			door_->render(Timer::getRoundTime(), getPosition(), ANGLE_NO, SCALE_NO, DROID_DOOR_TRANSLATE,
				DrawCenter::BOTTOMCENTER, fadeColor_, -1, -1);
			break;
		case DROID_OPENING:
			door_->render(Timer::getRoundTime(), getPosition(), ANGLE_NO, SCALE_NO, DROID_DOOR_TRANSLATE,
				DrawCenter::BOTTOMCENTER, fadeColor_, -1, 0);
			break;
		case DROID_GENERATING:
			door_->render(Timer::getRoundTime(), getPosition(), ANGLE_NO, SCALE_NO, DROID_DOOR_TRANSLATE,
				DrawCenter::BOTTOMCENTER, fadeColor_, 0, 0);
			break;
		case DROID_CLOSING:
			door_->render(Timer::getRoundTime(), getPosition(), ANGLE_NO, SCALE_NO, DROID_DOOR_TRANSLATE,
				DrawCenter::BOTTOMCENTER, fadeColor_, 0, -1);
			break;
		default:
			break;
		}

		FOR(generatedObjects_.size())
			generatedObjects_.at(i)->draw();
	}
}

void Droid::activateUpdate()
{
	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);

	switch (state_)
	{
	case DROID_APPEAR:
		fadeTimer_ += Timer::getRoundTime();
		if ((fadeColor_ & 0xFF000000) == 0xFF000000)
		{
			state_ = DROID_OPENING;
			fadeTimer_ = 0;
			Object::deactivate(PROPERTIES_EVATION);
		}
		else
		{
			DWORD newCCode = fadeTimer_ * 255 / DROID_FADE_TIME;
			fadeColor_ = fadeColor_ | (newCCode > 255 ? 255 << 24 : newCCode << 24) | 0x00ffffff;
		}
		break;

	case DROID_DISSAPEAR:
		fadeTimer_ += Timer::getRoundTime();

		if ((fadeColor_ & 0xFF000000) == 0x00000000)
		{
			state_ = DROID_PREPARING;
			fadeTimer_ = 0;
		}
		else
		{
			DWORD newCCode = 255 - fadeTimer_ * 255 / DROID_FADE_TIME;
			fadeColor_ = fadeColor_ & (newCCode < 0 ? 0 : newCCode << 24) | 0x00ffffff;
		}
		break;

	case DROID_OPENING:
		if (door_->isComplete())
		{
			state_ = DROID_GENERATING;
		}
		break;

	case DROID_GENERATING:
		timer_ += Timer::getRoundTime();
		generateTimer_ += Timer::getRoundTime();

		if (generateTimer_ >= DROID_GENERATE_DELAY)
		{
			generateTimer_ = 0;

			if (generateSide_ == Side::SIDE_LEFT)
			{
				Mobs* generateOne = new MiniDroid(getPosition() + DROID_MINI_GENERATE_POS_FIX(generateSide_), SCALE_FLIP_X);
				generatedObjects_.push_back(generateOne);
				generateSide_ = Side::SIDE_RIGHT;
			}
			else
			{
				Mobs* generateOne = new MiniDroid(getPosition() + DROID_MINI_GENERATE_POS_FIX(generateSide_), SCALE_NO);
				generatedObjects_.push_back(generateOne);
				generateSide_ = Side::SIDE_LEFT;
			}

			generatedObjects_.push_back(new ExplodeFire(getPosition() + DROID_FIRE_GENERATE_POS_FIX));
		}

		if (timer_ >= DROID_GENERATE_TIME)
		{
			timer_ = 0;
			state_ = DroidState::DROID_CLOSING;
		}
		break;

	case DROID_CLOSING:
		if (door_->isComplete())
		{
			state_ = DROID_DISSAPEAR;
			Object::activate(PROPERTIES_EVATION);
		}
		break;

	case DROID_PREPARING:
		timer_ += Timer::getRoundTime();
		if (timer_ >= DROID_PREPARING_TIME)
		{
			srand((unsigned int)time(nullptr));
			timer_ = 0;
			_position.x = ((rand() % (randomRegion_.right - randomRegion_.left)) + randomRegion_.left);
			_position.y = ((rand() % (randomRegion_.top - randomRegion_.bottom)) + randomRegion_.bottom);

			*_objectDefRect = Default::createRectFromBottomCenter(getPosition(), DROID_BOUND_SIZE);
			state_ = DroidState::DROID_APPEAR;
		}
		break;

	default:
		break;
	}
}

void Droid::deadUpdate()
{
	FOR(generatedObjects_.size())
	{
		generatedObjects_.at(i)->damaged(BULLET_DAMAGE_MASSIVE);
	}
}

void Droid::deadEffect()
{
	srand((unsigned int)time(nullptr));

	FOR(10)
	{
		D3DXVECTOR3 explopos = getPosition();
		explopos.y += (rand() % (int)DROID_BOUND_SIZE.height);
		explopos.x += (rand() % (int)DROID_BOUND_SIZE.width) - ((int)DROID_BOUND_SIZE.width / 2);

		EffectManager::addExplode(explopos, i * DEFAULT_FRAME_TIME);
	}

	FOR(10)
	{
		D3DXVECTOR3 explopos = basePos_;
		explopos.y += (rand() % (int)staticBase_->getFrameSize().height);
		explopos.x -= (rand() % (int)staticBase_->getFrameSize().width) - ((int)staticBase_->getFrameSize().width / 2);

		EffectManager::addExplode(explopos, i * DEFAULT_FRAME_TIME + (DEFAULT_FRAME_TIME / 2));
	}
}

void Droid::setBasicProperties()
{
	Boss::setBasicProperties();
	state_ = DROID_DISSAPEAR;
	fadeColor_ = 0xFFFFFFFF;
	_heathPoints = DROID_LIFE_POINTS;
}

bool Droid::affect(Object* obj, DWORD key)
{
	if (this->isTouchable() && !isDead() && !obj->isImmortal() && obj->isTouchable())
		return Boss::affect(obj, key);

	FOR(generatedObjects_.size())
		 generatedObjects_.at(i)->affect(obj, key);

	return false;
}

void Droid::update()
{
	Mobs::update();

	FOR(generatedObjects_.size())
	{
		generatedObjects_.at(i)->update();

		if (generatedObjects_.at(i)->isTerminated())
		{
			generatedObjects_.erase(generatedObjects_.begin() + i);
			i--;
		}
	}
}