#include "BulletL.h"
T6_Sprite* BulletL::spriteBulletL_;

BulletL::BulletL() :Bullet()
{
	positionBulletL_ = position_;
	m_velocity = new Velocity(5,0);
	if (bulletBag_ > 0)
		spriteBulletL_ = new T6_Sprite(LINK_BULLET_L, COLUMNS_BULLET_L, ROWS_BULLET_L);
};

bool BulletL::setIsAlly()
{
	return true;
};
void BulletL::update()
{
	positionBulletL_.x += (m_velocity->x);
	positionBulletL_.y += (m_velocity->y);
};
void BulletL::draw()
{
	spriteBulletL_->render(0,positionBulletL_);
};
bool BulletL::hitEnemy()
{
	return false;
};