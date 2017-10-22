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
	RECT newTrackRect;
	int rectTop;
	int rectLeft;
	int rectRight;
	int rectBottom;
	switch (currentDirection)
	{
		case Direction::Left:
			rectTop = this->X + WindowOption::PLAYER_HEIGHT;
			rectLeft = this->Y - (WindowOption::PATH_WIDTH / 2); //allign to path
			rectRight = rectTop + length;
			rectBottom = this->Y - 3 * (WindowOption::PATH_WIDTH / 2);
			newTrackRect = { rectTop, rectLeft - 2, rectRight, rectBottom + 2 };
			break;
		case Direction::Right:
			rectTop = this->X - WindowOption::PLAYER_HEIGHT;
			rectLeft = this->Y + (WindowOption::PATH_WIDTH / 2); //allign to path
			rectRight = rectTop - length;
			rectBottom = this->Y + 3 * (WindowOption::PATH_WIDTH / 2);
			newTrackRect = { rectTop, rectLeft + 2, rectRight, rectBottom - 2 };
			break;
		case Direction::Up:
			rectTop = this->X + (WindowOption::PATH_WIDTH / 2);
			rectLeft = this->Y + WindowOption::PLAYER_HEIGHT; 
			rectRight = this->X + 3 * (WindowOption::PATH_WIDTH / 2);
			rectBottom = rectLeft + length;
			newTrackRect = { rectTop + 2, rectLeft, rectRight - 2, rectBottom };
			break;
		case Direction::Down:
			rectTop = this->X - (WindowOption::PATH_WIDTH / 2); //todo
			rectLeft = this->Y - WindowOption::PLAYER_HEIGHT;
			rectRight = this->X - 3 * (WindowOption::PATH_WIDTH / 2);
			rectBottom = rectLeft - length;
			newTrackRect = { rectTop -2, rectLeft, rectRight + 2, rectBottom };
			break;
	}
	FillRect(hdc, &newTrackRect, trackBrush);
}

void Player::Move(LPPOINT points, HDC hdc) {
	int stepLength = 1;
	int change = stepLength * speed;
	switch (currentDirection)
	{
	case Direction::Left:
		this->X -= change;
		points[1].x -= change;
		points[2].x -= change;
		break;
	case Direction::Right:
		this->X += change;
		points[1].x += change;
		points[2].x += change;
		break;
	case Direction::Up:
		this->Y -= change;
		points[1].y -= change;
		points[2].y -= change;
		break;
	case Direction::Down:
		this->Y += change;
		points[1].y += change;
		points[2].y += change;
		break;
	}
	points[0] = { this->X, this->Y };
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

bool Player::IsOpositeDirection(Direction direction) {
	switch (direction)
	{
		case Direction::Left:
			return currentDirection == Direction::Right;
		case Direction::Right:
			return currentDirection == Direction::Left;
		case Direction::Up:
			return currentDirection == Direction::Down;
		case Direction::Down:
			return currentDirection == Direction::Up;
	}
}

void Player::Turn(LPPOINT points, Direction direction, HDC hdc) {

	switch (direction)
	{
	case Left:
		if (currentDirection == Direction::Up) {
			this->X -= WindowOption::PLAYER_HEIGHT - 3 * WindowOption::PATH_WIDTH / 2 + 2;
			this->Y += WindowOption::PLAYER_HEIGHT + WindowOption::PATH_WIDTH / 2 + 2;
		}
		else { //down
			this->X -= WindowOption::PLAYER_HEIGHT + WindowOption::PATH_WIDTH / 2 + 2;// -3 * WindowOption::PATH_WIDTH / 2 + 2;
			this->Y -= WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
		}

		points[1] = { this->X, this->Y - WindowOption::PLAYER_WIDTH };//upperright
		points[2] = { this->X + WindowOption::PLAYER_HEIGHT, this->Y };//lowerleft
		break;
	case Right:
		if (currentDirection == Direction::Up) {
			this->X += WindowOption::PLAYER_HEIGHT + WindowOption::PATH_WIDTH;
			this->Y += WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
		}
		else { // down
			this->X += WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH ;
			this->Y -= WindowOption::PLAYER_HEIGHT + 3 * WindowOption::PATH_WIDTH / 2 - 2;
		}
		

		points[1] = { this->X, this->Y + WindowOption::PLAYER_WIDTH };
		points[2] = { this->X - WindowOption::PLAYER_HEIGHT, this->Y };//lowerleft
		break;
	}

	switch (direction)
	{
	case Up:
		if (currentDirection == Direction::Left) {
			this->X += WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
			this->Y -= WindowOption::PLAYER_HEIGHT + WindowOption::PATH_WIDTH;
		}
		else { //right
			this->X -= WindowOption::PLAYER_HEIGHT + 3 * WindowOption::PATH_WIDTH / 2 - 2;
			this->Y -= WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH;
		}
		

		points[1] = { this->X + WindowOption::PLAYER_WIDTH, this->Y };//upperright
		points[2] = { this->X, this->Y + WindowOption::PLAYER_HEIGHT };//lowerleft
		break;
	case Down:
		if (currentDirection == Direction::Left) {
			this->X += WindowOption::PLAYER_HEIGHT + 3 * WindowOption::PATH_WIDTH / 2 - 2;
			this->Y += WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH;
		}
		else { //right
			this->X -= WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
			this->Y += WindowOption::PLAYER_HEIGHT + WindowOption::PATH_WIDTH;
		}

		points[1] = { this->X - WindowOption::PLAYER_WIDTH, this->Y };//upperright
		points[2] = { this->X, this->Y - WindowOption::PLAYER_HEIGHT };//lowerleft
		break;
	}

	points[0] = { this->X, this->Y }; //upperleft corner;
	currentDirection = direction;
}
