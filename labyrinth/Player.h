#ifndef PLAYER_H
#define PLAYER_H

#include "WindowOptions.h"
#include "Direction.h"
#include "Bitmap.h"

class Player
{
public:
    Player(Bitmap figure, HBRUSH trackBrush, RECT position);
    ~Player();

	void TryMakeTurn();
	void ReduceSpeed();
	void UpSpeed();
	bool IsDead();
	void Move();

	int speed;
	RECT position;
	Direction currentDirection;

private:
	void DrawTrack();


	HBRUSH trackBrush;
	Bitmap figure;
};
#endif