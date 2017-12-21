#ifndef PLAYER_H
#define PLAYER_H

#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <tchar.h>
#include <cassert>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include "glm.hpp"

#include "WindowOptions.h"
#include "Direction.h"
#include "Bitmap.h"
#include "MemoryDrawer.h"
#include "OpenGLDrawer.h"

class Player
{
public:
	Player() {};
	Player(Bitmap figure, HBRUSH trackBrush, int x, int y, RECT field);
	~Player() {};

	struct LineDescription
	{
		POINT beginPoint;
		POINT endPoint;
		LineDescription(){}
	};


	void Turn(LPPOINT points, Direction direction);
	void ReduceSpeed();
	void UpSpeed();
	bool CheckIsDead(std::vector<std::shared_ptr<Player>> allPlayers);
	int Move(LPPOINT points, bool drawTrack, std::shared_ptr<OpenGlDrawer> drawer);
	RECT GetCurrentRectNormalized();
	bool IsCollizedWithPlayerRect(RECT rect);

	int GetPointCode(RECT rect, POINT p);
	bool IsLineCrossedRect(RECT rect, std::shared_ptr<LineDescription> lineDesc);

	bool IsOpositeDirection(Direction direction);

	RECT field;
	int speed;
	int X; //center positions
	int Y;
	Direction currentDirection;
	Bitmap figure;
	bool isDead;
	std::vector<std::shared_ptr<LineDescription>> trackPoints;
	bool afterTurn;

private:
	void DrawTrack(std::shared_ptr<OpenGlDrawer> drawer, int length);

	HBRUSH trackBrush;
};
#endif