#ifndef __STATE_H__
#define __STATE_H__

#include <time.h>

#include "DX9Framework\Keyboard.h"
#include "DX9Framework\Sprite.h"
#include "GameDefaultConstant.h"

class Character;


enum CharacterStatus {
	ON_GROUND = 0,
	JUMP_UP,
	FALL_DOWN,
	LIE_DOWN,
	DIE,
	SWIM,
	NUM_OF_CHARACTER_STATUS
};

#define IS_UPDATING_Y(A) ((A == CharacterStatus::FALL_DOWN) || (A == CharacterStatus::JUMP_UP) || (A == CharacterStatus::DIE))
#define FALL_IN_WATER_TIME	(EFFECT_WATER_FLOW_FRAME_TIME * EFFECT_WATER_FLOW_COLUMNS * EFFECT_WATER_FLOW_ROWS)

#define BOUND_SIZE_ON_GROUND	Size(16.0, 30.0)
#define BOUND_SIZE_FALLING		Size(16.0, 30.0)
#define BOUND_SIZE_JUMPING		Size(20.0, 20.0)
#define BOUND_SIZE_LIE			Size(26.0, 10.0)
#define BOUND_SIZE_DIE			Size(20.0, 20.0)
#define BOUND_SIZE_SWIM			Size(12.0, 13.0)

#pragma region Pure Virtual Base State

class State
{
public:
	virtual void			update() = 0;
	virtual void			draw(Texture* head, Texture* leg, DWORD fadedColor) = 0;
	virtual void			enter() = 0;
	virtual void			controling() = 0;
	virtual int				computeShootAngle(int& baseAngle) = 0;
	virtual CharacterStatus	getIndex() = 0;
	virtual Size			getBoundSize() = 0;
	virtual D3DXVECTOR3		getDrawPos(Texture* leg);
	virtual bool			isWaterCollis();

	static void initializeCharacterState(Keyboard* keyboard, Character* player);
	static void releaseCharacterState();
	static void reset();
	static Keyboard* getControler();
	static State* getState(int index);

protected:
	static	void		collisBullet();

protected:
	static Keyboard* _controler;
	static Character*	_player;
	static State*		_stateList[CharacterStatus::NUM_OF_CHARACTER_STATUS];

private:
	friend Character;
	static void				dead();
};

#pragma endregion

#pragma region Being On Ground

class OnGround : public State
{
public:
	void update();
	void draw(Texture* head, Texture* leg, DWORD fadedColor);
	void enter();
	void controling();
	CharacterStatus	getIndex();
	int	 computeShootAngle(int& baseAngle);

private:
	friend class Character;
	void setMoveSprite();

	virtual Size getBoundSize();

};

#pragma endregion

#pragma region Lying down

class LieDown : public State
{
public:
	void update();
	void draw(Texture* head, Texture* leg, DWORD fadedColor);
	void enter();
	void controling();
	CharacterStatus	getIndex();
	int	 computeShootAngle(int& baseAngle);

	virtual Size getBoundSize();

};

#pragma endregion

#pragma region Jumping

class JumpUp : public State
{
public:
	void update();
	void draw(Texture* head, Texture* leg, DWORD fadedColor);
	void enter();
	void controling();
	CharacterStatus	getIndex();
	int	 computeShootAngle(int& baseAngle);

	virtual Size getBoundSize();

};

#pragma endregion

#pragma region Falling from a tile

class FallDown : public State
{
public:
	void update();
	void draw(Texture* head, Texture* leg, DWORD fadedColor);
	void enter();
	void controling();
	CharacterStatus	getIndex();
	int	 computeShootAngle(int& baseAngle);

	virtual Size getBoundSize();

};

#pragma endregion

#pragma region Die state

#define DIE_SOUND	_T("Resource//Soundtrack//Dead.wav")

class Die : public State
{
public:
	void update();
	void draw(Texture* head, Texture* leg, DWORD fadedColor);
	void enter();
	void controling();
	CharacterStatus	getIndex();
	int	 computeShootAngle(int& baseAngle);

	virtual Size getBoundSize();

	virtual bool isWaterCollis();

private:
	double _showTime;
};

#pragma endregion

#pragma region Swim state

enum SwimSubState
{
	FALL_IN_WATER = 0,
	SWIMING,
	DIVE,
	CLIMB_UP,
	NUM_OF_SUB_STATE
};

class Swim : public State
{
public:
	~Swim();
	void update();
	void draw(Texture* head, Texture* leg, DWORD fadedColor);
	void enter();
	void controling();
	CharacterStatus	getIndex();
	int	 computeShootAngle(int& baseAngle);

private:
	void substateSwitchTo(SwimSubState newState);
	void setShootSprite();

	virtual Size getBoundSize();

private:
	DWORD _floatingTime;
	DWORD _fallInWaterTime;
	int _floatPixel;
	bool _subStateCompleted;
	SwimSubState _currentSwimState;

	static Sprite* _waterWave;
};

#pragma endregion

#include "Character.h"
#endif