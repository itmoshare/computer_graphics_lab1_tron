#include "WindowOptions.h"

const int WindowOption::WIDTH = 742;
const int WindowOption::HEIGHT = 600;

const int WindowOption::StartPositionX = 0;
const int WindowOption::StartPositionY = 0;

const RECT WindowOption::MAP_CANVAS_RECT = { 0,  0, WindowOption::WIDTH,  WindowOption::HEIGHT };

std::vector<wchar_t*> WindowOption::ButtonLabels = []() -> std::vector<wchar_t*> {
    std::vector<wchar_t*> names;
    names.push_back(_T("North"));
    names.push_back(_T("East"));
    names.push_back(_T("South"));
    names.push_back(_T("West"));
    return names;
}();

std::vector<std::tuple<int, int>> WindowOption::ButtonPositions = []() -> std::vector<std::tuple<int,int>> {
    int x = 350;
    int y = 100;
    std::vector<std::tuple<int,int>> v; 
    v.push_back(std::make_pair(x,y));  // north
    v.push_back(std::make_pair(x + (WindowOption::ButtonWidth/2), y + (WindowOption::ButtonHeight)));  // east
    v.push_back(std::make_pair(x , y + (WindowOption::ButtonHeight * 2)));  // south
    v.push_back(std::make_pair(x - (WindowOption::ButtonWidth/2), y + (WindowOption::ButtonHeight)));  // west
    return v; 
}();

const int WindowOption::ButtonWidth  = 50;
const int WindowOption::ButtonHeight = 24;

const int WindowOption::TILE_WIDTH = 16;
const int WindowOption::TILE_HEIGHT = 16;

const int WindowOption::WALL_WIDTH = 60;
const double WindowOption::WALL_HEIGHT = 46.6;
const double WindowOption::SMALL_WALL_WIDTH = 22.5;
const double WindowOption::PATH_WIDTH = 10;

const int WindowOption::HORIZONTAL_WALL_COUNT = 9;
const int WindowOption::VERTICALL_WALL_COUNT = 9;

const int WindowOption::BORDER_WIDTH = 20;

bool WindowOption::IsRunning = true;