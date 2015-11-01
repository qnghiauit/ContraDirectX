#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#pragma warning(disable : 4250)

#include "MoveObject.h"
#include "ShootObject.h"
#include "State.h"
#include "Map.h"
#include "ScoreManager.h"

#include "DX9Framework\DefaultMethods.h"
#include "DX9Framework\Camera.h"

#define CHARACTER_IMMORTAL_TIME	3000
#define CHARACTER_BULLET_IMMORTAL_TIME	8000
#define CHARACTER_EXTRA_LIFE_RESOURCE_LINK	(tstring(_T("Resource//Miscellaneous//ExtraLife")) + EXTENSION_PICTURE)
#define CHARACTER_NUM_OF_MAX_LIFE_ICON	4
#define CHARACTER_LIFE_ICON_Y_FIX	3
#define CHARACTER_LIFE_ICON_X_FIX	3
#define CHARACTER_LIFE_ICON_DISTANCE_TO_LEFT	5

enum CharacterStatus_Head {
	UP90 = 0,
	UP45,
	STRAIGHT,
	DOWN45,
	RUN_HEAD,
	SWIM_STILL,
	JUMP,
	FALL,
	LIE,
	DEAD,
	DIE_ALREADY,
	CLIMB,
	NUM_OF_C_HEAD_STATUS
};

#define RAPID_SHOOT_BONUS_SPEED	50
#define RESPAW_IMMORTAL_TIME	5000

enum CharacterStatus_Leg {
	STAND = 0,
	RUN_LEG,
	NUM_OF_C_LEG_STATUS
};

class Character : public virtual MoveObject, public virtual ShootObject
{
public:
	Character(D3DXVECTOR3 position);
	~Character();

	using MoveObject::getVelo;

	virtual void	update();
	virtual void	draw();
			void	updateDefRect(Size boundRange);
			void	setStatus(State* newState);
			void	switchLegStatusTo(unsigned int newStatus);
			void	dead();
			int		reset();
			Bullet* shoot();
			bool	checkIfReadyToClimbFromSwim();
			void	bonusLife();

	virtual void	setBasicProperties();
			int		getCurrentLegSpriteFrame();
			void	setLegFrame(int frame);
			State*	getCurrentStatus();
			int		getLifeRemaining();
			vector<CollisEvent*>	updateMoveableWithCollision();

			//UPDATE
			void	activateRapidShoot();
			void	activateImmortal();
			void	activateBoom();

	friend class State;
protected:
	int				computeShootAngle();
	D3DXVECTOR3		computeShootPoint();

private:
	void			collisionUpdate();

	virtual bool  affect(Object* obj, DWORD key);

	virtual bool isTerminated();

private:
	State*			_currentState;
	Texture**	_spriteCharLeg;
	int				_life;
	unsigned int	_status_leg;
	int				_bonusBulletSpeed;
	int				_immortalTime;
	bool			_immortalBulletActivate;
};

#endif