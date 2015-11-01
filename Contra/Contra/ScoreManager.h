#ifndef __SCORE_MANAGER_H__
#define __SCORE_MANAGER_H__

#define EXTRA_LIFE_SCORE_CAP	10000
#define DEFAULT_HIGH_SCORE		20000

class Character;

class ScoreManager
{
public:
	static bool increase(int score);
	static unsigned long getScore();
	static unsigned long getHighScore();
	static void initialize(Character* player);
	static void reset();

private:
	static ScoreManager* get();
	ScoreManager();
	~ScoreManager();

	static ScoreManager* _instance;

private:
	unsigned long _score;
	unsigned long _highScore;
	Character* _player;
};

#endif // ScoreManager_h__
