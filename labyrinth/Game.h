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


#include "WindowOptions.h"
#include "Command.h"
#include "Bitmap.h"
#include "Player.h"


class Game {
public:
    Game() {};
    ~Game() {};

    bool LoadBitmapFromFile(const std::wstring filename, std::string resourceName);
    void InitializeGraphics(HWND window);    
    void BeginGraphics();
    void DrawBitmap(Bitmap bitmap, int x, int y)  const;
    void DrawString(const std::wstring text, COLORREF color, int x, int y) const;
    void DrawPlayers() const;
    void Render(const double interpolation);
    void EndGraphics();
    void FreeBitmap(Bitmap bitmap);
    void ShutdownGraphics();

    void Start();
    void MovePlayer(Command command);
    void ProcessInput(Command command);    
    void CheckWinningCondition();
	

    // GDI-related stuff
    HWND window;
    int windowWidth;
    int windowHeight;

    HDC backbufferDC;
    HBITMAP backbufferBitmap;

	HDC backgroundBufferDC;
	HBITMAP backgroundBufferBitmap;

    HDC bitmapDC;

    struct GDIBitmap
    {
        HBITMAP handle;
        int width;
        int height;

        GDIBitmap()
            : handle(0)
        {
        }
    };
    // EOF GDI

	std::shared_ptr<Player> player;
	std::vector<Player> computerPlayers;

    std::map<std::string, Bitmap> bitmapDictionary;
    std::map<std::string, std::wstring> fileDictionary;
    std::vector<GDIBitmap> gdiBitmaps;
    std::vector<Bitmap> bitmaps;
	std::vector<RECT> walls;

	std::vector<int> path_xs;
	std::vector<int> path_ys;

    Bitmap playerBitmap;
    int score;
	int computerPlayersCount;
    HGDIOBJ oldObject;
	HGDIOBJ oldObject2;

private: 
	void DrawBackground(int windowWidth, int WindowHeight, HDC hdc);
	void DrawWall(double x, double y, double width, double height, HBRUSH brush, HDC hdc);
	void DrawSmallWall(double &x, double y, HBRUSH brush, HDC hdc);
	void DrawBigWall(double &x, double y, HBRUSH brush, HDC hdc);
	void InitPlayers();
};

#endif