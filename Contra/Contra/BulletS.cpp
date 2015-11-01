#include "BulletS.h"
T6_Sprite* BulletS::spriteBulletS_;

BulletS::BulletS() :Bullet()
{
	positionBulletS_ = position_;
	m_velocity = new Velocity(5,0);
	if (bulletBag_ > 0)
		spriteBulletS_ = new T6_Sprite(LINK_BULLET_S, COLUMNS_BULLET_S, ROWS_BULLET_S);
};

BulletS::~BulletS()
{
	delete m_velocity;
	delete spriteBulletS_;
};

bool BulletS::setIsAlly()
{
	return true;
};
void BulletS::update()
{
	positionBulletS_.x += (m_velocity->x);
	positionBulletS_.y += (m_velocity->y);
};
void BulletS::draw()
{
	spriteBulletS_->render(0, positionBulletS_);
};
bool BulletS::hitEnemy()
{
	return false;
};