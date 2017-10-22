#ifndef WINDOW_OPTIONS_H
#define WINDOW_OPTIONS_H

#include <vector>
#include <tuple>
#include <tchar.h>
#include <windows.h>
#include <chrono>

class WindowOption
{
public:
    static const int WIDTH;
    static const int HEIGHT;
    static const int StartPositionX;
    static const int StartPositionY;
    static const int TILE_WIDTH;
    static const int TILE_HEIGHT;

	static const int WALL_WIDTH;
	static const double WALL_HEIGHT;
	static const double SMALL_WALL_WIDTH;
	static const int PATH_WIDTH;

	static const int HORIZONTAL_WALL_COUNT;
	static const int VERTICALL_WALL_COUNT;

	static const int BORDER_WIDTH;

	static const int PLAYER_WIDTH;
	static const int PLAYER_HEIGHT;



    static std::vector<std::tuple<int, int>> ButtonPositions;
    static std::vector<wchar_t*> ButtonLabels;
    static std::vector<int> ButtonIDs;

    static std::chrono::high_resolution_clock clock;

    static const int ButtonWidth;
    static const int ButtonHeight;

    static bool IsRunning;
    
    static const RECT MAP_CANVAS_RECT;

    WindowOption();
    ~WindowOption();

private:

};

#endif