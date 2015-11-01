#include "BulletF.h"

T6_Sprite* BulletF::spriteBulletFirst_;
T6_Sprite* BulletF::spriteBulletSecond_;

BulletF::BulletF() : Bullet()
{
	positionBulletF_ = position_;
	m_velocity = new Velocity(5,0);
	if (bulletBag_ > 0)
		spriteBulletFirst_ = new T6_Sprite(LINK_BULLET_F, COLUMNS_BULLET_F, ROWS_BULLET_F);
	if (spriteBulletSecond_ > 0)
		spriteBulletSecond_ = new T6_Sprite(LINK_BULLET_F, COLUMNS_BULLET_F, ROWS_BULLET_F);
}

BulletF::~BulletF()
{
	delete m_velocity;
	delete spriteBulletFirst_;
	delete spriteBulletSecond_;
}

bool BulletF::setIsAlly()
{
	return true;;
};

void BulletF::update()
{
	positionBulletF_.x += (m_velocity->x);
	positionBulletF_.y += (m_velocity->y);
};

void BulletF::draw()
{
	spriteBulletFirst_->render(0,positionBulletF_);
	if (positionBulletF_+5 == position_)
	{
		spriteBulletSecond_->render(0, (positionBulletF_ - 5));
	}
};

bool BulletF::hitEnemy()
{
	return false;
};