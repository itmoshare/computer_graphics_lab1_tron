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

#include "WindowOptions.h"
#include "Direction.h"
#include "Bitmap.h"

class Player
{
public:
	Player() {};
	Player(Bitmap figure, HBRUSH trackBrush, int x, int y);
	~Player() {};

	void TryMakeTurn();
	void ReduceSpeed();
	void UpSpeed();
	bool IsDead();
	void Move(HDC hdc);

	int speed;
	int X; //center positions
	int Y;
	Direction currentDirection;
	Bitmap figure;


private:
	void DrawTrack(HDC hdc, int length);

	HBRUSH trackBrush;
};
#endif