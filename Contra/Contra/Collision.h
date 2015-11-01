#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Object.h"
#include "Timer.h"
#include "GameDefaultConstant.h"

#include "DX9Framework\DefaultStructure.h"
#include "DX9Framework\DefaultConstant.h"

using namespace std;

class CollisEvent
{
public:
	CollisEvent(Object* touchedObject, Direction direct, double time)
	{
		touchedObject_ = touchedObject;
		direct_ = direct;
		time_ = time;
	}
	~CollisEvent(){}

	static CollisEvent* isStandingOnSomething(vector<CollisEvent*>* collisEvents, RECT* objectRect = nullptr, vector<Object*>* exceptionTiles = nullptr)
	{
		FOR(collisEvents->size())
		{
			if (!collisEvents->at(i)->isIgnored(exceptionTiles))
				if (collisEvents->at(i)->getDirection() == Direction::UP)
				{
				if (objectRect != nullptr)
					if (objectRect->bottom < collisEvents->at(i)->getTouchedObject()->getRect().top)
						continue;

				return collisEvents->at(i);
				}
		}
		return nullptr;
	}

	bool isIgnored(vector<Object*>* exceptionEvents)
	{
		if (exceptionEvents != nullptr)
			FOR(exceptionEvents->size())
		{
			if (this->touchedObject_ == exceptionEvents->at(i))
				return true;
		}

		return false;
	}

	Object* getTouchedObject()
	{
		return touchedObject_;
	}

	Direction getDirection()
	{
		return direct_;
	}

	double getTime()
	{
		return time_;
	}

	static CollisEvent* getFirstHappenedEvent(const vector<CollisEvent*>& collisEvents)
	{
		if (collisEvents.size() > 0){
			int index = 0;
			double time = collisEvents[0]->getTime();

			FOR(collisEvents.size()) {
				if (collisEvents[i]->getTime() < time)
					index = i;
			}

			return collisEvents[index];
		}

		return nullptr;
	}

private:
	Object* touchedObject_;
	Direction direct_;
	double time_;
};



class Collision
{
public:
	// =======================================
	// Name: Collision:checkCollision()
	// Desc: Detect the if the ob1 tough the ob2 in the current update.
	// It's return the time ob1 touch ob2, and if it's touched each other,
	// the out_CollisDirect will return a value unequal with NONE.
	// Ex: ob1 hit the ob2's TOP then out_CollisDirect will return UP
	// =======================================
	static double checkCollision(Object* ob1, Object* ob2, Direction* out_CollisDirect)
	{

		D3DXVECTOR3 velocity = convertVelocity(ob1, ob2);
		if (velocity == SPEED_NO) {
			ASSIGN_PTR(out_CollisDirect, Direction::NONE);
			return -1;
		}

		Direction* result = new Direction(checkBound(ob1, ob2));

		if ((*result == Direction::UP		&& velocity.y < 0.0f) ||
			(*result == Direction::DOWN		&& velocity.y > 0.0f) ||
			(*result == Direction::LEFT		&& velocity.x > 0.0f) ||
			(*result == Direction::RIGHT	&& velocity.x < 0.0f)) {
			ASSIGN_PTR(out_CollisDirect, *result);
			SAFE_RELEASE(result);
			return 0;
		}

		double collisTime = collisAABB(ob1, ob2, velocity, result);
		ASSIGN_PTR(out_CollisDirect, *result);
		SAFE_RELEASE(result);

		return collisTime;
	}

private:
	// =======================================
	// Name: Collision:checkBound()
	// Desc: Detect the if the ob1 tough the ob2 in the current position.
	// **Without velocity.
	// =======================================
	static Direction checkBound(Object* ob1, Object* ob2)
	{
		if (Default::checkIfBounded(&ob1->getRect(), &ob2->getRect()))
			return detectSide(ob1, ob2);
		return Direction::NONE;
	}


	// =======================================
	// Name: Collision:detectSide()
	// Desc: Detect the direction to ob1 base on ob2.
	// =======================================
	static Direction detectSide(Object* ob1, Object* ob2)
	{
		D3DXVECTOR3 distance(Collision::distanceX(ob1, ob2), Collision::distanceY(ob1, ob2), 0.0F);
		if (MIN_ABS(distance.x, distance.y) == distance.x) {
			if (distance.x > 0)
				return Direction::LEFT;
			else if (distance.x < 0) return Direction::RIGHT;
			else if (Default::computeCenter(ob1->getRect()).x <
				Default::computeCenter(ob2->getRect()).x)
				return Direction::LEFT;
			else return Direction::RIGHT;
		}
		else
			if (distance.y > 0)
				return Direction::UP;
			else if (distance.y < 0)
				return Direction::DOWN;
			else if (Default::computeCenter(ob1->getRect()).y <
				Default::computeCenter(ob2->getRect()).y)
				return Direction::DOWN;
			else return Direction::UP;
	}


	// =======================================
	// Name: Collision:convertVelocity()
	// Desc: It's used to caculate the velocity for ob1 base on ob2 in static.
	// =======================================
	static D3DXVECTOR3 convertVelocity(Object* ob1, Object* ob2)
	{
		D3DXVECTOR3 result;
		result.x = ob1->getVelo()->x - ob2->getVelo()->x;
		result.y = ob1->getVelo()->y - ob2->getVelo()->y;
		result.z = ob1->getVelo()->z - ob2->getVelo()->z;

		return result;
	}


	// =======================================
	// Name: Collision:checkCollision()
	// Desc: Use the AABB collision detection algorithms to check if 2 object are bounded each other
	// in the current update.
	// =======================================
	static double collisAABB(Object* ob1, Object* ob2, D3DXVECTOR3 velocity_ob1, Direction* out_CollisDirect)
	{
		RECT rect1 = ob1->getRect();
		RECT rect2 = ob2->getRect();

		D3DXVECTOR3 trueSpeed = TRUE_SPEED(velocity_ob1, (float)Timer::getRoundTime());

		if (trueSpeed.x > 0) rect1.right += (LONG)trueSpeed.x;
		else				 rect1.left += (LONG)trueSpeed.x;

		if (trueSpeed.y > 0) rect1.top += (LONG)trueSpeed.y;
		else				 rect1.bottom += (LONG)trueSpeed.y;

		if (!Default::checkIfBounded(&rect1, &rect2)) {
			ASSIGN_PTR(out_CollisDirect, Direction::NONE);
			return -1;
		}

		D3DXVECTOR3 distance(distanceX(ob1, ob2), distanceY(ob1, ob2), 0.0F);
		D3DXVECTOR3 time(0, 0, 0);
		time.x = velocity_ob1.x != 0.0F ? SEC_TO_MICROSEC(distance.x / velocity_ob1.x) : -1.0F;
		time.y = velocity_ob1.y != 0.0F ? SEC_TO_MICROSEC(distance.y / velocity_ob1.y) : -1.0F;

		if ((time.x > time.y && time.y >= 0) || time.x < 0) {
			if (velocity_ob1.y > 0.0F && ob1->getRect().top <= ob2->getRect().bottom)
			{
				ASSIGN_PTR(out_CollisDirect, Direction::DOWN);
			}
			else if (velocity_ob1.y < 0.0F && ob1->getRect().bottom >= ob2->getRect().top)
			{
				ASSIGN_PTR(out_CollisDirect, Direction::UP);
			}
			else { ASSIGN_PTR(out_CollisDirect, Direction::NONE); }
			return time.y;
		}
		else {
			if (velocity_ob1.x > 0.0F && ob1->getRect().right <= ob2->getRect().left)
			{
				ASSIGN_PTR(out_CollisDirect, Direction::LEFT);
			}
			else if (velocity_ob1.x < 0.0F && ob1->getRect().left >= ob2->getRect().right)
			{
				ASSIGN_PTR(out_CollisDirect, Direction::RIGHT);
			}
			else { ASSIGN_PTR(out_CollisDirect, Direction::NONE); }
			return time.x;
		}
	}

	// =======================================
	// Name: Collision:distanceY()
	// Desc: Compute the shortest distance between ob1 and ob2 base on Y-axis.
	// =======================================
	static float distanceY(Object* ob1, Object* ob2)
	{
		float First = (float)(ob2->getRect().bottom - ob1->getRect().top);
		float Second = (float)(ob2->getRect().top - ob1->getRect().bottom);

		return (float)MIN_ABS(First, Second);
	}

	// =======================================
	// Name: Collision:distanceX()
	// Desc: Compute the shortest distance between ob1 and ob2 base on X-axis.
	// =======================================
	static float distanceX(Object* ob1, Object* ob2)
	{
		float First = (float)(ob2->getRect().left - ob1->getRect().right);
		float Second = (float)(ob2->getRect().right - ob1->getRect().left);

		return (float)MIN_ABS(First, Second);
	}
};

#endif