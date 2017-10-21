#include "Game.h"

bool Game::LoadBitmapFromFile(const std::wstring filename, std::string resourceName)
{
    Bitmap bitmap;
    GDIBitmap gdi;
    gdi.handle = (HBITMAP)LoadImage(0, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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

void Game::DrawWall(double x, double y, double width, double height, HBRUSH brush, HDC hdc) {
	RECT wall = { x, y, x + width, y + height };
	FillRect(hdc, &wall, brush);
	walls.push_back(wall);
}

void Game::DrawSmallWall(double &x, double y, HBRUSH brush, HDC hdc) {
	Game::DrawWall(x, y, WindowOption::SMALL_WALL_WIDTH, WindowOption::WALL_HEIGHT, brush, hdc);
	x += WindowOption::SMALL_WALL_WIDTH;
}
void Game::DrawBigWall(double &x, double y, HBRUSH brush, HDC hdc) {
	Game::DrawWall(x, y, WindowOption::WALL_WIDTH, WindowOption::WALL_HEIGHT, brush, hdc);
	x += WindowOption::WALL_WIDTH;
}

void Game::DrawBackground(int windowWidth, int WindowHeight, HDC hdc) {
	//black background
	RECT backRect = { 0, 0, windowWidth, windowHeight };
	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &backRect, blackBrush);

	//Draw border
	RECT borderUpRect = { 0, 0, windowWidth, WindowOption::BORDER_WIDTH };
	RECT borderDownRect = {0,  windowHeight-WindowOption::BORDER_WIDTH, windowWidth, windowHeight };
	RECT borderLeftRect = { 0, 0, WindowOption::BORDER_WIDTH, windowHeight };
	RECT borderRightRect = { windowWidth - WindowOption::BORDER_WIDTH, 0, windowWidth, windowHeight };
	HBRUSH borderBrush = CreateSolidBrush(RGB(91, 120, 125));
	FillRect(hdc, &borderUpRect, borderBrush);
	FillRect(hdc, &borderDownRect, borderBrush);
	FillRect(hdc, &borderLeftRect, borderBrush);
	FillRect(hdc, &borderRightRect, borderBrush);


	//Draw walls
	HBRUSH wallBrush = CreateSolidBrush(RGB(0, 2, 39));
	double currentX = WindowOption::BORDER_WIDTH;
	double currentY = WindowOption::BORDER_WIDTH;
	for (int i = 0; i < WindowOption::VERTICALL_WALL_COUNT; i++) {
		Game::DrawSmallWall(currentX, currentY, wallBrush, hdc);
		if (i == 0)
			path_xs.push_back(currentX);
		currentX += WindowOption::PATH_WIDTH;
		for (int j = 0; j < WindowOption::VERTICALL_WALL_COUNT; j++)
		{
			DrawBigWall(currentX, currentY, wallBrush, hdc);
			if (i == 0)
				path_xs.push_back(currentX);
			currentX += WindowOption::PATH_WIDTH;
		}
		Game::DrawSmallWall(currentX, currentY, wallBrush, hdc);
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
    // Double buffering, borrowed from GDI-provided wrapper in next assignment
    this->window = window;

    HDC hdc = GetDC(this->window);
   

	

    RECT windowSize = { 0 };
    windowSize = WindowOption::MAP_CANVAS_RECT;

	RECT clientRect;
	GetClientRect(this->window, &clientRect);

    windowWidth = clientRect.right - clientRect.left;
    windowHeight = clientRect.bottom - clientRect.top;
	
	backgroundBufferDC = CreateCompatibleDC(hdc);
	backgroundBufferBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
	oldObject2 = SelectObject(backgroundBufferDC, backgroundBufferBitmap);

	backbufferDC = CreateCompatibleDC(hdc);
	backbufferBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
	oldObject = SelectObject(backbufferDC, backbufferBitmap);


	//DrawBackground(windowWidth, windowHeight, hdc);
	DrawBackground(windowWidth, windowHeight, backgroundBufferDC);

	


	Game::InitPlayers();


	bitmapDC = CreateCompatibleDC(backgroundBufferDC);
    //backbufferBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
    // Store old object so that we don't leak.

    //SetBkMode(backbufferDC, TRANSPARENT);
	
    //bitmapDC = CreateCompatibleDC(hdc);

	//DrawPlayers();

    //// Load assets
    //LoadBitmapFromFile(std::wstring(_T("wall.bmp")), Resource::WallTile);
    //LoadBitmapFromFile(std::wstring(_T("grass.bmp")), Resource::GrassTile);
    //LoadBitmapFromFile(std::wstring(_T("start.bmp")), Resource::StartTile);
    //LoadBitmapFromFile(std::wstring(_T("end.bmp")), Resource::EndTile);
    //LoadBitmapFromFile(std::wstring(_T("player.bmp")), Resource::PlayerTile);

    //this->player = std::make_shared<Player>(0, 0);
    //player->resource = Resource::PlayerTile;

    //// Initialize entities
    //this->entities.push_back(player);
}

void Game::BeginGraphics()
{
	////saved bakcground, not redrawing it
	//HDC windowDC = GetDC(window);
	//BitBlt(windowDC, 0, 0, windowWidth, windowHeight, backgroundBufferDC, 0, 0, SRCCOPY);
	//ReleaseDC(window, windowDC);
	
	// draw on drawn bakcground
	/*backbufferDC = CreateCompatibleDC(backgroundBufferDC);
	backbufferBitmap = CreateCompatibleBitmap(backgroundBufferDC, windowWidth, windowHeight);
	oldObject = SelectObject(backbufferDC, backbufferBitmap);

	bitmapDC = CreateCompatibleDC(backgroundBufferDC);*/
	/*MaskBlt(backbufferDC, 0, 0, windowWidth, windowHeight, backgroundBufferDC, 0, 0,
		backgroundBufferBitmap, windowWidth, windowHeight, SRCCOPY);*/
	BitBlt(backbufferDC, 0, 0, windowWidth, windowHeight, backgroundBufferDC, 0, 0, SRCCOPY);
}



void Game::DrawBitmap(Bitmap bitmap, int x, int y) const
{
    const GDIBitmap& gdi = gdiBitmaps.at(bitmap.index);
    SelectObject(bitmapDC, gdi.handle);
    BitBlt(backbufferDC, x, y, gdi.width, gdi.height, bitmapDC, 0, 0, SRCCOPY);
}

void Game::DrawString(const std::wstring text, COLORREF color, int x, int y) const
{
    SetTextColor(backbufferDC, color);
    TextOut(backbufferDC, x, y, text.c_str(), text.size());
}

void Game::DrawPlayers() const {
	
	DrawBitmap(player->figure, player->X, player->Y);
	
	for (int i = 0; i < computerPlayers.size(); i++) {
		DrawBitmap(computerPlayers[i].figure, computerPlayers[i].X, computerPlayers[i].Y);
	}
}

void Game::Render(const double interpolation) {

	BeginGraphics();
	DrawPlayers();
	EndGraphics();
}

void Game::EndGraphics()
{
	/*MaskBlt(backbufferDC, 0, 0, windowWidth, windowHeight, backgroundBufferDC, 0, 0,
		backgroundBufferBitmap, windowWidth, windowHeight, SRCCOPY);*/

    // Blit-block transfer to the main device context
    HDC windowDC = GetDC(window);
    BitBlt(windowDC, 0, 0, windowWidth, windowHeight, backbufferDC, 0, 0, SRCCOPY);
    ReleaseDC(window, windowDC);	
}

void Game::FreeBitmap(Bitmap bitmap)
{
    // Find the resource, from the bitmap, to free
    //auto findResult = std::find_if(std::begin(bitmapDictionary), std::end(bitmapDictionary), [&](const std::pair<Resource, Bitmap>& pair) {
    //    return pair.second.index == bitmap.index;
    //});
    //Resource foundKey;
    //if (findResult != std::end(bitmapDictionary))
    //{
    //    foundKey = findResult->first;

    //    // Remove file and bitmap
    //    bitmapDictionary.erase(foundKey);
    //    fileDictionary.erase(foundKey);
    //}

    //// fria minnet som upptas av en bitmap
    //if (gdiBitmaps[bitmap.index].handle != 0)
    //{
    //    DeleteObject(gdiBitmaps[bitmap.index].handle);
    //    gdiBitmaps[bitmap.index].handle = 0;
    //}
}

void Game::ShutdownGraphics()
{
    for (auto& bm : bitmaps) {
        FreeBitmap(bm);
    }

    // fria alla resurser som anvönds av grafiksystemet
    DeleteDC(this->bitmapDC);

    DeleteObject(this->backbufferBitmap);
    SelectObject(this->backbufferDC, oldObject);
    DeleteDC(this->backbufferDC);
}

void Game::InitPlayers() {
	//human player
	LoadBitmapFromFile(std::wstring(_T("tron_player.bmp")), "player");

	int centerPathIndex = path_xs.size() / 2 - 1;

	int playerX = path_xs[centerPathIndex] - (WindowOption::PATH_WIDTH / 2);
	int playerY = windowHeight - WindowOption::BORDER_WIDTH - WindowOption::WALL_HEIGHT;

	this->player = std::make_shared<Player>(
		bitmapDictionary.at("player"), 
		CreateSolidBrush(RGB(0, 0, 0)),
		playerX, playerY);

	int count = 1;
	int positionDiff = 0;
	int cplayerY = WindowOption::BORDER_WIDTH;
	for (int i = 0; i < count; i++) {
		LoadBitmapFromFile(std::wstring(_T("computer_player.bmp")), "computerPlayer" + std::to_string(i));
		
		if (i % 2 == 0) {
			positionDiff = -positionDiff;
		}
		else {
			positionDiff = -positionDiff+1;
		}
		int cplayerX = path_xs[centerPathIndex + positionDiff] - (WindowOption::PATH_WIDTH / 2);
		
		Player cplayer(
			bitmapDictionary.at("computerPlayer" + std::to_string(i)),
			CreateSolidBrush(RGB(100, 100, 100)), 
			cplayerX, cplayerY);
		this->computerPlayers.push_back(cplayer);
	}

}

// Start game
void Game::Start() {
    /*int startPosition = find(currentLevel->tiles.begin(), currentLevel->tiles.end(), Resource::StartTile) - this->levels[0]->tiles.begin();
    int x = startPosition % currentLevel->TILES_PER_COLUMN;
    int y = startPosition / currentLevel->TILES_PER_COLUMN;
    player->x = x * WindowOption::TILE_WIDTH;
    player->y = y * WindowOption::TILE_HEIGHT;
    score = 0;*/
}

// Move player, assume level's contour is made out of walls,
// otherwise perform boundary checks.
void Game::MovePlayer(Command command) {
	player->Move(GetDC(this->window));
}

void Game::ProcessInput(Command command) {
    switch (command) {
    case Command::MoveRight:

    case Command::SpeedUp:
		player->currentDirection = Direction::Up;
		MovePlayer(command);
		break;
    case Command::Slow:
		player->currentDirection = Direction::Down;
		MovePlayer(command);
		break;
    case Command::MoveLeft:
        MovePlayer(command);
        break;
    case Command::Restart:
        Start();
    }
}

void Game::CheckWinningCondition() {
    /*int x = (int)player->x / WindowOption::TILE_WIDTH;
    int y = (int)player->y / WindowOption::TILE_HEIGHT;
    auto position = x + (y * currentLevel->TILES_PER_COLUMN);
    auto tile = currentLevel->tiles[position];
    if (tile == Resource::EndTile) {
        std::wstring finishText(_T("You found the exit "));
        finishText += std::to_wstring(score);
        finishText += std::wstring(_T(" in steps!\r\n"));
        finishText += std::wstring(_T("Would you like to play again?"));
        if (MessageBox(this->window, finishText.c_str(), _T("Freedom at last"), MB_YESNO) == IDYES) {
            Start();
        }
        else {
            exit(1);
        }
    }*/
}