#include "ScoreManager.h"
#include "Character.h"

ScoreManager::ScoreManager()
{
	_score = 0;
	_highScore = DEFAULT_HIGH_SCORE;
}

ScoreManager::~ScoreManager()
{

}

bool ScoreManager::increase(int score)
{
	get()->_score += score;

	if ((get()->_score % EXTRA_LIFE_SCORE_CAP) < score)
	{
		get()->_player->bonusLife();
		return true;
	}

	return false;
}

ScoreManager* ScoreManager::get()
{
	if (_instance == nullptr)
	{
		_instance = new ScoreManager();
	}

	return _instance;
}

unsigned long ScoreManager::getScore()
{
	return get()->_score;
}

unsigned long ScoreManager::getHighScore()
{
	if (get()->_highScore < get()->_score)
	{
		get()->_highScore = get()->_score;
	}

	return get()->_highScore;
}

void ScoreManager::initialize(Character* player)
{
	get()->_player = player;
}

void ScoreManager::reset()
{
	get()->_score = 0;
}

ScoreManager* ScoreManager::_instance;
