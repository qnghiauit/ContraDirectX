#ifndef __BOSS_H__
#define __BOSS_H__

#include "Mobs.h"
#include "FreeSound.h"

#define SCORE_BOSS	10000
#define BOSS_DEAD_SOUND	_T("Resource//Soundtrack//BossExplode.wav")
#define STAGE_CLEAR_SOUND	_T("Resource//Soundtrack//StageClear.wav")
#define BOSS_DEAD_TIME	5000	// 5 seconds
#define TIME_TO_NEXT_STAGE	9000	// 4 seconds after totally dead

class Boss :
	public Mobs
{
public:
	virtual ~Boss();

	virtual unsigned int getScore();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual void deadEffect();

	virtual void setBasicProperties();

	virtual void update();

			DWORD getTimeCounter();

protected:
			void assignSubPart(Mobs* newPart);


private:
	vector<Mobs*> _subParts;
	DWORD _timeCounter;
	bool _stageClearPlayed;
};

#endif // Boss_h__