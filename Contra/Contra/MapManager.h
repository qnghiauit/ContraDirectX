#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include "Map.h"

class MapManager
{
public:
	static MapManager& getInstance();
	static void update();
	static void nextStage();

private:
	MapManager();
	~MapManager();

private:
	Map* _currentMap;
	int _currentStage;
};


#endif // !__MAP_MANAGER_H__
