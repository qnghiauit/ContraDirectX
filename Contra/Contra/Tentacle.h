#ifndef __TENTACLE_H__
#define __TENTACLE_H__

#include "MetalOrb.h"
#include "CrystalOrb.h"

#define SCORE_TENTACLE	500
#define TENTACLE_NUM_OF_METAL_ORB	4
#define TENTACLE_UPDATE_TURN_TIME	5000
#define TENTACLE_WAVE_SPEED		360
#define TENTACLE_NUM_OF_UPDATE_TYPE	3
#define TENTACLE_ACTIVATE_SPEED	30
#define TENTACLE_ORB_DISTANCE	16.0F
#define TENTACLE_ORB_PREPARE_TIME	FRAME_TIME
#define TENTACLE_LIFE_POINT	50

struct TOrb
{
	MetalOrb* orb;
	float angle;
	float timeCounter;

	TOrb(MetalOrb* tOrb, float tAngle, float tTime)
	{
		orb = tOrb;
		angle = tAngle;
		timeCounter = tTime;
	}

	~TOrb()
	{
		SAFE_RELEASE(orb);
	}
};

class Tentacle : public Mobs
{
	typedef void(Tentacle::*WaveSpeedProcessFunction)();
	typedef bool(Tentacle::*updateFunc)();
public:
	Tentacle(D3DXVECTOR3 position, int angleInDegree, Object* target);
	virtual ~Tentacle();

	virtual unsigned int getScore();

	virtual void draw();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual void setBasicProperties();

	virtual bool affect(Object* obj, DWORD key);

	virtual bool isTerminated();

	virtual bool isDead();

private:
	bool updateWaveType();	// The Tentacle wave up and down
	bool updateRoundType();	// The Tentacle gyrate around
	bool updatePointType();	// The Tentacle directly pointing to target

	void waveSpeedProcessLeft();
	void waveSpeedProcessRight();

	void setOrbPosByTheirAngle();
	void preparingTimeProcess();

	void activating();

	virtual void waitUpdate();

	virtual void deadEffect();

private:
	vector<TOrb*> _orbs;
	vector<D3DXVECTOR3> _movements;
	CrystalOrb*	_head;
	double _headDirection;
	Object* _target;
	int _waveSpeed;
	int _routSpeed;
	int _currentUpdatefunction;
	WaveSpeedProcessFunction _waveSpeedProcess;
	bool _isActivated;

	DWORD _timerCounter;
};

#endif // TENTACLE_h__