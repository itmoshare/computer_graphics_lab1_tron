#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <chrono>
#include <memory>

#include "WindowOptions.h"
#include "Game.h"

HWND btnNorth = NULL;
HWND btnEast = NULL;
HWND btnSouth = NULL;
HWND btnWest = NULL;

const int ID_OPEN = 1;
const int ID_QUIT = 2;
const int ID_ABOUT = 3;


std::shared_ptr<Game> game;
std::weak_ptr<Game> wGame;


HMENU CreateMainMenu()
{
    HMENU file = CreateMenu();
    AppendMenu(file, MF_STRING, ID_OPEN, _T("&New"));
    AppendMenu(file, MF_SEPARATOR, 0, 0);
    AppendMenu(file, MF_STRING, ID_QUIT, _T("&Quit"));

    HMENU help = CreateMenu();
    AppendMenu(help, MF_STRING, ID_ABOUT, _T("&About"));

    HMENU main = CreateMenu();
    AppendMenu(main, MF_POPUP, (UINT_PTR)file, _T("&File"));
    AppendMenu(main, MF_POPUP, (UINT_PTR)help, _T("&Help"));
    
    return main;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    // TODO: fix duplication, casting char/ID to enum directly.
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_QUIT:
            PostQuitMessage(0);
            return 0;
            break;
        case ID_ABOUT: {
            std::wstring about(   _T("Tron is legendary game. Bad copy made by Шумеев А.А. гр. Р3418 \r\n"));
            MessageBox(hwnd, about.c_str(), _T("About tron"), MB_OK);
        }
            break;
        case ID_OPEN:
           // game->Start();
            break;
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_UP:
            game->ProcessInput(Direction::Up);
            break;
        case VK_RIGHT:
            game->ProcessInput(Direction::Right);
            break;
        case VK_DOWN:
            game->ProcessInput(Direction::Down);
            break;
        case VK_LEFT:
            game->ProcessInput(Direction::Left);
            break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);        
		EndPaint(hwnd, &ps);
        return 0;
    }
    break;
    case WM_ERASEBKGND:
        return 1;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Render() {
    game->Render();
}

void processInput(const std::shared_ptr<Game>& game, MSG* msg) {
    while (PeekMessage(msg, NULL, NULL, NULL, PM_REMOVE)) {
        if (msg->message == WM_QUIT) {
            WindowOption::IsRunning = false;
        }

        TranslateMessage(msg);
        DispatchMessage(msg);
    }
}

void ShowString(LPCWSTR message, HWND hwnd) {
	HDC hdc = GetWindowDC(hwnd);
	SetTextColor(hdc, 0x00000000);
	SetBkMode(hdc, TRANSPARENT);
	RECT rect = { 100, 100, 100, 100 };
	DrawText(hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
	DeleteDC(hdc);
}

void DrawLoseText(HWND hwnd) {
	LPCWSTR message = L"You win";
	ShowString(message, hwnd);
}

void DrawWinText(HWND hwnd) {
	LPCWSTR message = L"You lose";
	ShowString(message, hwnd);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    wchar_t className[] = _T("TronClass");
    wchar_t windowName[] = _T("Tron");

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        className,
        windowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        WindowOption::StartPositionX, WindowOption::StartPositionY,
        WindowOption::WIDTH, WindowOption::HEIGHT,
        NULL,
        CreateMainMenu(),
        hInstance, NULL);


    UpdateWindow(hwnd);
    ShowWindow(hwnd, nCmdShow);

    game = std::make_shared<Game>();
    game->InitializeGraphics(hwnd);

    MSG msg = { 0 };

    const double FRAMES_PER_SEC = 60.0;
    const double SEC_PER_UPDATE = 1.0 / FRAMES_PER_SEC;

    auto previousTime = WindowOption::clock.now();
    double lag = 0.0;
    while (WindowOption::IsRunning) {

        auto currentTime = WindowOption::clock.now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - previousTime).count();
        previousTime = currentTime;

        processInput(game, &msg);
		Render();

        if (SEC_PER_UPDATE - deltaTime > 0)
            Sleep(SEC_PER_UPDATE - deltaTime);
    }

    return msg.wParam;
}
