#include "Player.h"

Player::Player(Bitmap figure, HBRUSH trackBrush, int x, int y) {
	this->figure = figure;
	this->trackBrush = trackBrush;
	this->X = x;
	this->Y = y;

	this->speed = 1;
	this->currentDirection = Direction::Up;
}

void Player::DrawTrack(HDC hdc, int length) {
	//RECT newTrackRect;
	//switch (currentDirection)
	//{
	//	case Direction::Left:
	//		int rectTop = this->X + WindowOption::PLAYER_HEIGHT;
	//		int rectLeft = this->Y - (WindowOption::PATH_WIDTH / 2); //allign to path
	//		int rectRight = rectTop + length;
	//		int rectBottom = this->Y - 3 * (WindowOption::PATH_WIDTH / 2);
	//		newTrackRect = { rectTop, rectLeft, rectRight, rectBottom };
	//		break;
	//	case Direction::Right:
	//		int rectTop = this->X - WindowOption::PLAYER_HEIGHT;
	//		int rectLeft = this->Y + (WindowOption::PATH_WIDTH / 2); //allign to path
	//		int rectRight = rectTop - length;
	//		int rectBottom = this->Y + 3 * (WindowOption::PATH_WIDTH / 2);
	//		newTrackRect = { rectTop, rectLeft, rectRight, rectBottom };
	//		break;
	//	case Direction::Up:
	//		int rectTop = this->X - (WindowOption::PATH_WIDTH / 2);
	//		int rectLeft = this->Y + WindowOption::PLAYER_HEIGHT; 
	//		int rectRight = this->X + 3 * (WindowOption::PATH_WIDTH / 2);
	//		int rectBottom = rectTop + length;
	//		newTrackRect = { rectTop, rectLeft, rectRight, rectBottom };
	//		break;
	//	case Direction::Down:
	//		int rectTop = this->X - (WindowOption::PATH_WIDTH / 2); //todo
	//		int rectLeft = this->Y + WindowOption::PLAYER_HEIGHT;
	//		int rectRight = this->X + 3 * (WindowOption::PATH_WIDTH / 2);
	//		int rectBottom = rectTop + length;
	//		newTrackRect = { rectTop, rectLeft, rectRight, rectBottom };
	//		break;
	//}
	//FillRect(hdc, &newTrackRect, trackBrush);
}

void Player::Move(HDC hdc) {
	int stepLength = 1;
	int change = stepLength * speed;
	switch (currentDirection)
	{
	case Direction::Left:
		this->X -= change;
		break;
	case Direction::Right:
		this->X += change;
		break;
	case Direction::Up:
		this->Y -= change;
		break;
	case Direction::Down:
		this->Y += change;
		break;
	}
	DrawTrack(hdc, change);
}

void Player::ReduceSpeed() {
	if (speed > 1)
		speed--;
}

void Player::UpSpeed() {
	if (speed < 15)
		speed++;
}

