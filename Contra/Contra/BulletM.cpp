#include "BulletM.h"

T6_Sprite* BulletM:: spriteBulletM_;

BulletM::BulletM() :BulletBasic()
{
	if (bulletBag_ > 0)
		spriteBulletM_ = new T6_Sprite(LINK_BULLET_M, COLUMNS_BULLET_M, ROWS_BULLET_M);
};

BulletM::~BulletM()
{
	delete spriteBulletM_;
};

void BulletM::update()
{
	positionBulletBasic_.x += (m_velocity->x);
	positionBulletBasic_.y += (m_velocity->y);
};
void BulletM::draw()
{
	spriteBulletM_->render(0,positionBulletBasic_);
};
bool BulletM::hitEnemy()
{
	return false;
};