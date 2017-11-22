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
            std::wstring about(   _T("Tron made by Шумеев А.А. гр. Р3418 \r\n"));
            MessageBox(hwnd, about.c_str(), _T("About tron"), MB_OK);
        }
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

bool AnimateNextFrame(int desiredFrameRate)
{
	static double lastTime = 0.0f;
	double elapsedTime = 0.0;

	// Get current time in seconds  (milliseconds * .001 = seconds)
	double currentTime = GetTickCount() * 0.001;

	// Get the elapsed time by subtracting the current time from the last time
	elapsedTime = currentTime - lastTime;

	// Check if the time since we last checked is over (1 second / framesPerSecond)
	if (elapsedTime > (1.0 / desiredFrameRate))
	{
		// Reset the last time
		lastTime = currentTime;

		// Return TRUE, to animate the next frame of animation
		return true;
	}

	// We don't animate right now.
	return false;
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
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        WindowOption::StartPositionX, WindowOption::StartPositionY,
        WindowOption::WIDTH, WindowOption::HEIGHT,
        NULL,
        CreateMainMenu(),
        hInstance, NULL);


    UpdateWindow(hwnd);
    ShowWindow(hwnd, nCmdShow);

    game = std::make_shared<Game>();
    game->InitializeGraphics(hwnd);

	MSG msg;
    while (WindowOption::IsRunning) {

      
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)					// If the message wasn't to quit
				break;
			TranslateMessage(&msg);						// Find out what the message does
			DispatchMessage(&msg);						// Execute the message
		}
		else											// if there wasn't a message
		{
			if (AnimateNextFrame(60))					// Make sure we only animate 60 FPS
			{
				Render();							// Render the scene every frame
				
			}
			else
			{
				Sleep(1);								// Let other processes work
			}
		}
    }

    return msg.wParam;
}
