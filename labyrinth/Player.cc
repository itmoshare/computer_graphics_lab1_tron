#include "Player.h"

#define LEFT  1  /* двоичное 0001 */
#define RIGHT 2  /* двоичное 0010 */
#define BOT   4  /* двоичное 0100 */
#define TOP   8  /* двоичное 1000 */

Player::Player(Bitmap figure, HBRUSH trackBrush, int x, int y, RECT field) {
	this->figure = figure;
	this->trackBrush = trackBrush;
	this->X = x;
	this->Y = y;
	this->field = field;

	this->speed = 1;
	this->currentDirection = Direction::Up;
	this->isDead = false;
	this->afterTurn = false;
}

void Player::DrawTrack(HDC hdc, int length) {
	RECT newTrackRect;
	int rectLeft;
	int rectTop;
	int rectRight;
	int rectBottom;
	switch (currentDirection)
	{
		case Direction::Left:
			rectLeft = this->X + WindowOption::PLAYER_HEIGHT;
			rectTop = this->Y - (WindowOption::PATH_WIDTH / 2); //allign to path
			rectRight = rectLeft + length;
			rectBottom = this->Y - 3 * (WindowOption::PATH_WIDTH / 2);
			newTrackRect = { rectLeft, rectTop - 2, rectRight, rectBottom + 2 };
			
			if (afterTurn || trackPoints.size() == 0) {
				std::shared_ptr<LineDescription> trackLine = std::make_shared<LineDescription>();
				trackLine->beginPoint = { newTrackRect.left, newTrackRect.top };
				trackLine->endPoint = { newTrackRect.right, newTrackRect.bottom + WindowOption::PATH_WIDTH - 4 };
				trackPoints.push_back(trackLine);
			}
			else {
				std::shared_ptr<LineDescription> prevLine = trackPoints.at(trackPoints.size() - 1);
				//change left point to new
				prevLine->beginPoint.x = newTrackRect.left;
				prevLine->beginPoint.y = newTrackRect.top;
			}
			break;
		case Direction::Right:
			rectLeft = this->X - WindowOption::PLAYER_HEIGHT;
			rectTop = this->Y + (WindowOption::PATH_WIDTH / 2); //allign to path
			rectRight = rectLeft - length;
			rectBottom = this->Y + 3 * (WindowOption::PATH_WIDTH / 2);
			newTrackRect = { rectLeft, rectTop + 2, rectRight, rectBottom - 2 };

			if (afterTurn || trackPoints.size() == 0) {
				std::shared_ptr<LineDescription> trackLine = std::make_shared<LineDescription>();
				trackLine->beginPoint = { newTrackRect.right, newTrackRect.bottom };
				trackLine->endPoint = { newTrackRect.left, newTrackRect.top + WindowOption::PATH_WIDTH - 4};
				trackPoints.push_back(trackLine);
			}
			else {
				std::shared_ptr<LineDescription> prevLine = trackPoints.at(trackPoints.size() - 1);
				//change right point to new
				prevLine->endPoint.x = newTrackRect.left;
				prevLine->endPoint.y = newTrackRect.top + WindowOption::PATH_WIDTH - 4;
			}
			break;
		case Direction::Up:
			rectLeft = this->X + (WindowOption::PATH_WIDTH / 2);
			rectTop = this->Y + WindowOption::PLAYER_HEIGHT; 
			rectRight = this->X + 3 * (WindowOption::PATH_WIDTH / 2);
			rectBottom = rectTop + length;
			newTrackRect = { rectLeft + 2, rectTop, rectRight - 2, rectBottom };

			if (afterTurn || trackPoints.size() == 0) {
				std::shared_ptr<LineDescription> trackLine = std::make_shared<LineDescription>();
				trackLine->beginPoint =  { newTrackRect.left, newTrackRect.top }; //up is begin, because it is must be less
				trackLine->endPoint = { newTrackRect.right, newTrackRect.bottom };
				trackPoints.push_back(trackLine);
			}
			else {
				std::shared_ptr<LineDescription> prevLine = trackPoints.at(trackPoints.size() - 1);
				//change up point to new
				prevLine->beginPoint.x = newTrackRect.left;
				prevLine->beginPoint.y = newTrackRect.top;
			}

			break;
		case Direction::Down:
			rectLeft = this->X - (WindowOption::PATH_WIDTH / 2); //todo
			rectTop = this->Y - WindowOption::PLAYER_HEIGHT;
			rectRight = this->X - 3 * (WindowOption::PATH_WIDTH / 2);
			rectBottom = rectTop - length;
			newTrackRect = { rectLeft -2, rectTop, rectRight + 2, rectBottom };

			if (afterTurn || trackPoints.size() == 0) {
				std::shared_ptr<LineDescription> trackLine = std::make_shared<LineDescription>();
				trackLine->beginPoint =  { newTrackRect.right, newTrackRect.bottom };//up is begin, because it is must be less
				trackLine->endPoint = { newTrackRect.left, newTrackRect.top };
				trackPoints.push_back(trackLine);
			}
			else {
				std::shared_ptr<LineDescription> prevLine = trackPoints.at(trackPoints.size() - 1);
				//change down point to new
				prevLine->endPoint.x = newTrackRect.left;
				prevLine->endPoint.y = newTrackRect.top;
			}

			break;
	}
	FillRect(hdc, &newTrackRect, trackBrush);
	afterTurn = false;
	
}

void Player::Move(LPPOINT points, bool drawTrack, HDC hdc) {
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
	if(drawTrack)
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
	if (direction == currentDirection || IsOpositeDirection(direction))
		return;
	switch (direction)
	{
	case Left:
		if (currentDirection == Direction::Up) {
			this->X -= WindowOption::PLAYER_HEIGHT -   WindowOption::PATH_WIDTH / 2 -2;
			this->Y += WindowOption::PLAYER_HEIGHT + WindowOption::PATH_WIDTH + 3;
		}
		else { //down
			this->X -= WindowOption::PLAYER_HEIGHT +3 * WindowOption::PATH_WIDTH / 2 -2;// -3 * WindowOption::PATH_WIDTH / 2 + 2;
			this->Y -= WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
		}

		points[1] = { this->X, this->Y - WindowOption::PLAYER_WIDTH };//upperright
		points[2] = { this->X + WindowOption::PLAYER_HEIGHT, this->Y };//lowerleft
		break;
	case Right:
		if (currentDirection == Direction::Up) {
			this->X += WindowOption::PLAYER_HEIGHT + 3 * WindowOption::PATH_WIDTH / 2 - 2  ;
			this->Y += WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
		}
		else { // down
			this->X += WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
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
			this->Y += WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 -2;
		}
		else { //right
			this->X -= WindowOption::PLAYER_HEIGHT - WindowOption::PATH_WIDTH / 2 - 2;
			this->Y += WindowOption::PLAYER_HEIGHT + 3 * WindowOption::PATH_WIDTH / 2 - 2;
		}

		points[1] = { this->X - WindowOption::PLAYER_WIDTH, this->Y };//upperright
		points[2] = { this->X, this->Y - WindowOption::PLAYER_HEIGHT };//lowerleft
		break;
	}

	points[0] = { this->X, this->Y }; //upperleft corner;
	currentDirection = direction;
	afterTurn = true;
}

RECT Player::GetCurrentRectNormalized() {
	RECT rect;
	switch (currentDirection)
	{	
	case Left:
		rect = { this->X, this->Y - WindowOption::PLAYER_WIDTH, this->X + WindowOption::PLAYER_HEIGHT, this->Y };
		break;
	case Right:
		rect = { this->X - WindowOption::PLAYER_HEIGHT, this->Y, this->X, this->Y + WindowOption::PLAYER_WIDTH };
		break;
	case Down:
		rect = { this->X - WindowOption::PLAYER_WIDTH , this->Y - WindowOption::PLAYER_HEIGHT , this->X, this->Y };
		break;
	case Up:
		rect = { this->X, this->Y, this->X + WindowOption::PLAYER_WIDTH , this->Y + WindowOption::PLAYER_HEIGHT };
		break;
	}
	return rect;
}

bool Player::IsCollizedWithPlayerRect(RECT rect) {
	switch (currentDirection)
	{
	case Left:
		//return this->X > rect.left && this->X < rect.right && this->Y > 
		break;
	case Right:
		break;
	case Down:
		break;
	case Up:
		break;
	default:
		break;
	}
	return false;
}

bool IsBetween(int check, int left, int right) {
	return check > left && check < right;
}

int Player::GetPointCode(RECT r, POINT p) {
	return (p.x <= r.left ? LEFT : 0) +  /* +1 если точка левее пр€моугольника */ \
		(p.x >= r.right ? RIGHT : 0) +  /* +2 если точка правее пр€моугольника */\
		(p.y >= r.bottom ? BOT : 0) +  /* +4 если точка ниже пр€моугольника */  \
		(p.y <= r.top ? TOP : 0);     /* +8 если точка выше пр€моугольника */
}

bool Player::IsLineCrossedRect(RECT rect, std::shared_ptr<LineDescription> lineDesc) {
	int code_a, code_b; /* код концов отрезка */
	code_a = GetPointCode(rect, lineDesc->beginPoint);
	code_b = GetPointCode(rect, lineDesc->endPoint);

	return (code_a & code_b) == 0;	
}

bool AreRectsCollied(RECT rect1, RECT rect2) {
	return (rect1.left < rect2.right) && (rect1.right > rect2.left) &&
		(rect1.top < rect2.bottom) && (rect1.bottom > rect2.top);
}

bool Player::CheckIsDead(std::vector<std::shared_ptr<Player>> allPlayers, HDC hdc) {

	
	//border collissions
	if (X < field.left || X > field.right || Y < field.top || Y > field.bottom)
	{
		isDead = true;
		return isDead;
	}

	RECT playerRect = this->GetCurrentRectNormalized();

	for (int j = 0; j < allPlayers.size(); j++) {
		std::shared_ptr<Player> player = allPlayers[j];
		
		
		//tracks colission
		for(int i=0; i<player->trackPoints.size(); i++)
		{
			std::shared_ptr<LineDescription> lineDesc = player->trackPoints[i];
			
			if (IsLineCrossedRect(playerRect, lineDesc))
			{
				RECT rect = { lineDesc->beginPoint.x - 2, lineDesc->beginPoint.y, lineDesc->endPoint.x, lineDesc->endPoint.y };
				FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 0)));
				isDead = true;
				return true;
			}
		}

		//skip current player
		if (player->figure.index == this->figure.index)
			continue;
		RECT opRect = player->GetCurrentRectNormalized();
		if (AreRectsCollied(playerRect, opRect))
		{
			isDead = true;
			return true;
		}

	}
	
	isDead = false;
	return false;
}
