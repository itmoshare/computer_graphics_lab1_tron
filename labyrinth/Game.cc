#include "Game.h"

bool Game::LoadBitmapFromFile(const std::wstring filename, std::string resourceName)
{
    Bitmap bitmap;
    GDIBitmap gdi;
    gdi.handle = (HBITMAP)LoadImage(0, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (gdi.handle == 0)
    {
        return false;
    }

    // Read size
    BITMAP header;
    GetObject(gdi.handle, sizeof(header), &header);
    gdi.width = header.bmWidth;
    gdi.height = header.bmHeight;

    // cache data
    gdiBitmaps.push_back(gdi);
    bitmap.index = gdiBitmaps.size() - 1;
    bitmaps.push_back(bitmap);
    bitmapDictionary[resourceName] = bitmap; //todo
    fileDictionary[resourceName] = filename; //todo

    return true;
}

void Game::DrawWall(double x, double y, double width, double height, HBRUSH brush) {
	RECT wall = { x, y, x + width, y + height };
	memDrawer->DrawBackgroundRect(wall, brush);
	walls.push_back(wall);
}

void Game::DrawSmallWall(double &x, double y, HBRUSH brush) {
	Game::DrawWall(x, y, WindowOption::SMALL_WALL_WIDTH, WindowOption::WALL_HEIGHT, brush);
	x += WindowOption::SMALL_WALL_WIDTH;
}
void Game::DrawBigWall(double &x, double y, HBRUSH brush) {
	Game::DrawWall(x, y, WindowOption::WALL_WIDTH, WindowOption::WALL_HEIGHT, brush);
	x += WindowOption::WALL_WIDTH;
}

void Game::DrawBackground(int windowWidth, int WindowHeight) {
	//black background
	RECT backRect = { 0, 0, windowWidth, windowHeight };
	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	//memDrawer->DrawBackgroundRect(backRect, blackBrush);

	//Draw border
	RECT borderUpRect = { 0, 0, windowWidth, WindowOption::BORDER_WIDTH };
	RECT borderDownRect = {0,  windowHeight-WindowOption::BORDER_WIDTH, windowWidth, windowHeight };
	RECT borderLeftRect = { 0, 0, WindowOption::BORDER_WIDTH, windowHeight };
	RECT borderRightRect = { windowWidth - WindowOption::BORDER_WIDTH, 0, windowWidth, windowHeight };
	HBRUSH borderBrush = CreateSolidBrush(RGB(91, 120, 125));
	memDrawer->DrawBackgroundRect(borderUpRect, borderBrush);
	memDrawer->DrawBackgroundRect(borderDownRect, borderBrush);
	memDrawer->DrawBackgroundRect(borderLeftRect, borderBrush);
	memDrawer->DrawBackgroundRect(borderRightRect, borderBrush);


	//Draw walls
	HBRUSH wallBrush = CreateSolidBrush(RGB(0, 2, 39));
	double currentX = WindowOption::BORDER_WIDTH;
	double currentY = WindowOption::BORDER_WIDTH;
	for (int i = 0; i < WindowOption::VERTICALL_WALL_COUNT; i++) {
		Game::DrawSmallWall(currentX, currentY, wallBrush);
		if (i == 0)
			path_xs.push_back(currentX);
		currentX += WindowOption::PATH_WIDTH;
		for (int j = 0; j < WindowOption::VERTICALL_WALL_COUNT; j++)
		{
			DrawBigWall(currentX, currentY, wallBrush);
			if (i == 0)
				path_xs.push_back(currentX);
			currentX += WindowOption::PATH_WIDTH;
		}
		Game::DrawSmallWall(currentX, currentY, wallBrush);
		//Line drawn
		if (i != WindowOption::VERTICALL_WALL_COUNT - 1) {
			currentY += WindowOption::WALL_HEIGHT;
			path_ys.push_back(currentY);
			currentY += WindowOption::PATH_WIDTH;
			currentX = WindowOption::BORDER_WIDTH;
		}
		
	}
}


void Game::InitializeGraphics(HWND window)
{
    this->window = window;

	memDrawer = std::make_shared<OpenGlDrawer>();
   
    RECT windowSize = { 0 };
    windowSize = WindowOption::MAP_CANVAS_RECT;

	RECT clientRect;
	GetClientRect(this->window, &clientRect);

    windowWidth = clientRect.right - clientRect.left;
    windowHeight = clientRect.bottom - clientRect.top;
	
	memDrawer->OnInitializeGraphice(window, windowWidth, windowHeight);
	
	DrawBackground(windowWidth, windowHeight);


	Game::InitPlayers();
	//comment when memory drawer
	memDrawer->CreateTextures(gdiBitmaps.at(0).handle, gdiBitmaps.at(1).handle);
}

void Game::BeginGraphics() {
	memDrawer->OnBeginGraphics();
}



void Game::DrawBitmap(Bitmap bitmap, int x, int y) const
{
    const GDIBitmap& gdi = gdiBitmaps.at(bitmap.index);
	bool isPlayer = bitmap.index == 0;
	memDrawer->DrawGdi(gdi, isPlayer);
}



void Game::DrawPlayers() const {
	
	for (int i = 0; i < allPlayers.size(); i++)
	{
		this->DrawBitmap(allPlayers[i]->figure, allPlayers[i]->X, allPlayers[i]->Y);
	}
}



int moveDelay = 0;
void Game::MovePlayers() {
	if (moveDelay == 0) {
		
		for (int i = 0; i < allPlayers.size(); i++)
		{
			if (!allPlayers[i]->isDead) {
				GDIBitmap gdi = gdiBitmaps.at(allPlayers[i]->figure.index);
				allPlayers[i]->Move(gdi.points, true, memDrawer);
				gdiBitmaps[allPlayers[i]->figure.index] = gdi;
				allPlayers[i]->CheckIsDead(allPlayers);
			}
			else {
				int a = 3;
			}
		}
		
		moveDelay = 0;
	}
	else
		moveDelay--;
}

bool Game::IsPlayerWin() {
	for each (std::shared_ptr<Player> cplayer in computerPlayers)
	{
		if (!cplayer->isDead)
			return false;
	}
	return true;
}

bool Game::IsPlayerLose() {
	return this->player->isDead;
}



void Game::DrawWinGame() {

	memDrawer->DrawWinGame();
}

void Game::DrawLoseGame() {
	memDrawer->DrawLoseGame();     
}

void Game::Render() {

	BeginGraphics();
	DrawPlayers();

	if (this->IsPlayerWin()) {
		this->DrawWinGame();
	}
	else if (this->IsPlayerLose()) {
		this->DrawLoseGame();
	}
	else {
		
		ControlComputerPlayers();
		
		
		MovePlayers();
		
	}
	EndGraphics();
}

void Game::EndGraphics()
{
	memDrawer->OnEndGraphics();
}


void Game::InitGDI(int x, int y, int index, bool revert) {
	GDIBitmap gdi = gdiBitmaps.at(index);
	if (!revert) {
		gdi.points[0] = { x, y };
		gdi.points[1] = { x + gdi.width, y };
		gdi.points[2] = { x, y + gdi.height };
	}
	else {
		gdi.points[0] = { x + gdi.width, y + gdi.height };
		gdi.points[1] = { x, y + gdi.height };
		gdi.points[2] = { x + gdi.width, y };
	}
	
	gdiBitmaps[index] = gdi;
}

void Game::InitPlayers() {
	//human player
	LoadBitmapFromFile(std::wstring(_T("tron_player2.bmp")), "player");

	RECT fieldRect = { WindowOption::BORDER_WIDTH, WindowOption::BORDER_WIDTH, windowWidth - WindowOption::BORDER_WIDTH, windowHeight - WindowOption::BORDER_WIDTH };

	int centerPathIndex = path_xs.size() / 2 - 1;

	int playerX = path_xs[centerPathIndex] - (WindowOption::PATH_WIDTH / 2);
	int playerY = windowHeight - WindowOption::BORDER_WIDTH - WindowOption::WALL_HEIGHT;

	this->player = std::make_shared<Player>(
		bitmapDictionary.at("player"), 
		CreateSolidBrush(RGB(30, 136, 239)),
		playerX, playerY, fieldRect);

	InitGDI(playerX, playerY, player->figure.index, false);

	this->allPlayers.push_back(this->player);

	int count = 1;
	int positionDiff = 0;
	int cplayerY = WindowOption::BORDER_WIDTH;
	for (int i = 0; i < count; i++) {
		LoadBitmapFromFile(std::wstring(_T("computer_player2.bmp")), "computerPlayer" + std::to_string(i));
		
		if (i % 2 == 0) {
			positionDiff = -positionDiff;
		}
		else {
			positionDiff = -positionDiff+1;
		}
		int cplayerX = path_xs[centerPathIndex + positionDiff] - (WindowOption::PATH_WIDTH / 2);
		
		std::shared_ptr<Player> cplayer = std::make_shared<Player>(
			bitmapDictionary.at("computerPlayer" + std::to_string(i)),
			CreateSolidBrush(RGB(253, 216, 0)), 
			cplayerX + WindowOption::PLAYER_WIDTH, cplayerY + WindowOption::PLAYER_HEIGHT, fieldRect);
		cplayer->currentDirection = Direction::Down;
		InitGDI(cplayerX, cplayerY, bitmapDictionary.at("computerPlayer" + std::to_string(i)).index, true);

		this->computerPlayers.push_back(cplayer);
		this->allPlayers.push_back(cplayer);
	}

}

bool Game::CheckHorizontalPath(int y, int &path_y_out) {
	for each (int path_y in path_ys)
	{
		if (y <= path_y + WindowOption::PLAYER_HEIGHT / 3 && y >= path_y - WindowOption::PLAYER_HEIGHT / 3)
		{
			path_y_out = path_y;
			return true;
		}
	}
	return false;
}

bool Game::CheckVerticalPath(int x, int &path_x_out) {
	for each (int path_x in path_xs)
	{
		if (x <= path_x + WindowOption::PLAYER_HEIGHT / 3 && x >= path_x - WindowOption::PLAYER_HEIGHT / 3)
		{
			path_x_out = path_x;
			return true;
		}
	}
	return false;
}

bool Game::CanTurn(Direction direction, Player * player, int &path_x, int &path_y) {
	int x = player->X;
	int y = player->Y;

	switch (direction)
	{
	case Left:
		return CheckHorizontalPath(y, path_y);
		break;
	case Right:
		return CheckHorizontalPath(y, path_y);
		break;
	case Down:
		return CheckVerticalPath(x, path_x);
		break;
	case Up:
		return CheckVerticalPath(x, path_x);
		break;
	default:
		return false;
		break;
	}
}

void Game::TurnPlayer(Direction direction, Player * player) {

	int path_x = 0;
	int path_y = 0;
	
	if (CanTurn(direction, player, path_x, path_y)) {
		GDIBitmap gdBmp = gdiBitmaps.at(player->figure.index);

		player->currentDirection = direction;
		if (path_x == 0) {
			//y path
			switch (direction)
			{
			case Left:
				player->X -= path_y - player->Y;
				player->Y = path_y + (3 *WindowOption::PATH_WIDTH / 2);
				
				gdBmp.points[1] = { player->X, player->Y - WindowOption::PLAYER_WIDTH};//upperright
				gdBmp.points[2] = { player->X + WindowOption::PLAYER_HEIGHT, player->Y};//lowerleft
				break;
			case Right:
				
				player->X += 0;//WindowOption::PLAYER_WIDTH;//path_y - player->Y;
				player->Y = path_y - (WindowOption::PATH_WIDTH / 2);

				gdBmp.points[1] = { player->X, player->Y + WindowOption::PLAYER_WIDTH };
				gdBmp.points[2] = { player->X - WindowOption::PLAYER_HEIGHT, player->Y };//lowerleft
				break;
			}
		}
		else {
			//x path
			switch (direction)
			{
			case Up:
				player->X = path_x - WindowOption::PATH_WIDTH / 2;
				player->Y += path_x - player->X;

				gdBmp.points[1] = { player->X + WindowOption::PLAYER_WIDTH, player->Y };//upperright
				gdBmp.points[2] = { player->X, player->Y + WindowOption::PLAYER_HEIGHT };//lowerleft
				break;
			case Down:
				player->X = path_x + (3 * WindowOption::PATH_WIDTH / 2) ;
				player->Y -= path_x - player->X;

				gdBmp.points[1] = { player->X - WindowOption::PLAYER_WIDTH, player->Y };//upperright
				gdBmp.points[2] = { player->X, player->Y - WindowOption::PLAYER_HEIGHT };//lowerleft
				break;
			}
			
		}
		gdBmp.points[0] = { player->X, player->Y }; //upperleft corner;
		gdiBitmaps[player->figure.index] = gdBmp;
	}

}

void Game::TurnPlayerNoWalls(Direction direction, Player * player) {
		
	GDIBitmap gdBmp = gdiBitmaps.at(player->figure.index);

	player->currentDirection = direction;

	switch (direction)
	{
	case Left:
		player->X -= WindowOption::PLAYER_HEIGHT / 2;
		player->Y += WindowOption::PLAYER_HEIGHT;
		
		gdBmp.points[1] = { player->X, player->Y - WindowOption::PLAYER_WIDTH };//upperright
		gdBmp.points[2] = { player->X + WindowOption::PLAYER_HEIGHT, player->Y };//lowerleft
		break;
	case Right:

		player->X += WindowOption::PLAYER_HEIGHT;//WindowOption::PLAYER_WIDTH;//path_y - player->Y;

		gdBmp.points[1] = { player->X, player->Y + WindowOption::PLAYER_WIDTH };
		gdBmp.points[2] = { player->X - WindowOption::PLAYER_HEIGHT, player->Y };//lowerleft
		break;
	}

	switch (direction)
	{
	case Up:
		player->Y -= WindowOption::PLAYER_HEIGHT;

		gdBmp.points[1] = { player->X + WindowOption::PLAYER_WIDTH, player->Y };//upperright
		gdBmp.points[2] = { player->X, player->Y + WindowOption::PLAYER_HEIGHT };//lowerleft
		break;
	case Down:
		player->Y += WindowOption::PLAYER_HEIGHT;

		gdBmp.points[1] = { player->X - WindowOption::PLAYER_WIDTH, player->Y };//upperright
		gdBmp.points[2] = { player->X, player->Y - WindowOption::PLAYER_HEIGHT };//lowerleft
		break;
	}

	gdBmp.points[0] = { player->X, player->Y }; //upperleft corner;
	gdiBitmaps[player->figure.index] = gdBmp;
	

}

void Game::ProcessInput(Direction direction) {
	if (direction == player->currentDirection)
	{
		player->UpSpeed();
		return;
	}

	if (player->IsOpositeDirection(direction))
	{
		player->ReduceSpeed();
		return;
	}
	
	GDIBitmap gdBmp = gdiBitmaps.at(player->figure.index);
	this->player->Turn(gdBmp.points, direction);
	gdiBitmaps[player->figure.index] = gdBmp;

	// TurnPlayerNoWalls(direction, &*player);
}

bool RandomBool(int trueChanceReduction) {
	return (rand() > RAND_MAX / trueChanceReduction) ? false : true;
}

int afterMoveDelay = 2; // moves to wait until turn 
void Game::ControlComputerPlayers() {
	if (moveDelay != 0)
		return;
	if (afterMoveDelay != 0)
	{
		afterMoveDelay--;
		return;
	}
	else {
		afterMoveDelay = 2;
	}
	
	for each (std::shared_ptr<Player> cplayer in computerPlayers)
	{	
		if (cplayer->isDead)
			continue;

		bool increaseSpeed = RandomBool(200);
		if (increaseSpeed)
			cplayer->speed += 1;

		//move player to see if it will be dead. if yes, turn
		int cx = cplayer->X;
		int cy = cplayer->Y;//save current pos

		GDIBitmap gdiTemp = gdiBitmaps.at(cplayer->figure.index);
		//3 steps 
		cplayer->Move(gdiTemp.points, false, NULL);
		cplayer->Move(gdiTemp.points, false, NULL);
		cplayer->Move(gdiTemp.points, false, NULL);

		bool needToTurn = cplayer->CheckIsDead(allPlayers);

		//restore position etc
		cplayer->X = cx;
		cplayer->Y = cy;
		cplayer->isDead = false;

		if (needToTurn)
		{		
			Direction d1, d2;
			switch (cplayer->currentDirection)
			{
			case Left:
			case Right:
				d1 = Direction::Up;
				d2 = Direction::Down;
				break;
			case Up:
			case Down:
				d1 = Direction::Left;
				d2 = Direction::Right;
				break;
			}

			Direction randomDirection = static_cast<Direction>((rand() > RAND_MAX / 2) ? d1 : d2);

			GDIBitmap gdBmp = gdiBitmaps.at(cplayer->figure.index);
			cplayer->Turn(gdBmp.points, randomDirection);
			gdiBitmaps[cplayer->figure.index] = gdBmp;
		}
	}
}