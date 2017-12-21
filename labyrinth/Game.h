#ifndef GAME_H
#define GAME_H

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
#include "Bitmap.h"
#include "Player.h"
#include "MemoryDrawer.h"
#include "GDIBitmap.h"
#include "OpenGLDrawer.h"

class Game {
public:
    Game() {};
    ~Game() {};

    bool LoadBitmapFromFile(const std::wstring filename, std::string resourceName);
    void InitializeGraphics(HWND window);    
    void BeginGraphics();
    void DrawBitmap(Bitmap bitmap, int x, int y, Direction direction)  const;
    void DrawString(const std::wstring text, COLORREF color, int x, int y) const;
    void DrawPlayers() const;
    void Render();
    void EndGraphics();
	void InitGDI(int x, int y, int index, bool revert);

    void ProcessInput(Direction direction);    
	void MovePlayers();
	bool CheckHorizontalPath(int y, int &path_y_out);
	bool CheckVerticalPath(int x, int &path_x_out);
	bool CanTurn(Direction direction, Player * player, int &path_x, int &path_y);

	void TurnPlayer(Direction direction, Player * player);
	void TurnPlayerNoWalls(Direction direction, Player * player);
	bool IsPlayerWin();
	bool IsPlayerLose();

    // GDI-related stuff
    HWND window;
    int windowWidth;
    int windowHeight;

   

   
    // EOF GDI

	std::shared_ptr<OpenGlDrawer> memDrawer;

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Player>> computerPlayers;

    std::map<std::string, Bitmap> bitmapDictionary;
    std::map<std::string, std::wstring> fileDictionary;
    std::vector<GDIBitmap> gdiBitmaps;
    std::vector<Bitmap> bitmaps;
	std::vector<RECT> walls;

	std::vector<std::shared_ptr<Player>> allPlayers;

	std::vector<int> path_xs;
	std::vector<int> path_ys;

    Bitmap playerBitmap;
    int score;
	int computerPlayersCount;
   
	//glm
	int computerChange;
	int playerChange;

	int coputerStartX;
	int computerStartY;
	int playerStartX;
	int playerStartY;


private: 
	void DrawBackground(int windowWidth, int WindowHeight);
	void DrawWall(double x, double y, double width, double height, HBRUSH brush);
	void DrawSmallWall(double &x, double y, HBRUSH brus);
	void DrawBigWall(double &x, double y, HBRUSH brush);
	void InitPlayers();
	void DrawWinGame();
	void DrawLoseGame();
	void SetGameoverFontSettings();
	void ControlComputerPlayers();
};

#endif