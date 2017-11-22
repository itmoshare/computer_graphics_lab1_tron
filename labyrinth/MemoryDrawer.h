#ifndef MEMORYDRAWER_H
#define MEMORYDRAWER_H

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
#include "Bitmap.h"
#include "GDIBitmap.h"

class MemoryDrawer {
public:
	MemoryDrawer() {};
	~MemoryDrawer() {};
	void OnInitializeGraphice(HWND window, int windowWidth, int windowHeight);
	void OnBeginGraphics();
	void OnEndGraphics();
	void DrawGdi(GDIBitmap gdi);
	void DrawString(const std::wstring text, COLORREF color, int x, int y) const;

	void DrawWinGame();
	void DrawLoseGame();

	void DrawRect(RECT rect, HBRUSH brush);
	void DrawBackgroundRect(RECT rect, HBRUSH brush);

	void SetGameoverFontSettings();

	int windowWidth;
	int windowHeight;

	HDC defaultHdc;

	HWND window;
	HDC backbufferDC;
	HBITMAP backbufferBitmap;

	HDC backgroundBufferDC;
	HBITMAP backgroundBufferBitmap;

	HDC bitmapDC;

	HGDIOBJ oldObject;
	HGDIOBJ oldObject2;
	HGDIOBJ def_font;


private:
};
#endif