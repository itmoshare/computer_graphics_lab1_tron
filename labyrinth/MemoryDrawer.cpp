#include "MemoryDrawer.h"

void MemoryDrawer::OnInitializeGraphice(HWND window, int windowWidth, int windowHeight)
{
	this->window = window;
	this->defaultHdc = GetDC(window);
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	//save drawn background to not redraw it everytime
	backgroundBufferDC = CreateCompatibleDC(defaultHdc);
	backgroundBufferBitmap = CreateCompatibleBitmap(defaultHdc, windowWidth, windowHeight);
	oldObject2 = SelectObject(backgroundBufferDC, backgroundBufferBitmap);

	// Double buffering
	backbufferDC = CreateCompatibleDC(defaultHdc);
	backbufferBitmap = CreateCompatibleBitmap(defaultHdc, windowWidth, windowHeight);
	oldObject = SelectObject(backbufferDC, backbufferBitmap);

	bitmapDC = CreateCompatibleDC(backgroundBufferDC);

}

void MemoryDrawer::OnBeginGraphics()
{
	//copy drawn background to bufferdc
	BitBlt(backbufferDC, 0, 0, windowWidth, windowHeight, backgroundBufferDC, 0, 0, SRCCOPY);
}

void MemoryDrawer::OnEndGraphics()
{
	// Blit-block transfer to the main device context
	HDC windowDC = GetDC(window);
	BitBlt(windowDC, 0, 0, windowWidth, windowHeight, backbufferDC, 0, 0, SRCCOPY);
	ReleaseDC(window, windowDC);	
	DeleteObject(def_font);
}

void MemoryDrawer::DrawGdi(GDIBitmap gdi)
{	
	SelectObject(bitmapDC, gdi.handle);
	PlgBlt(backbufferDC, gdi.points, bitmapDC, 0, 0, gdi.width, gdi.height, NULL, 0, 0);
}

void MemoryDrawer::DrawString(const std::wstring text, COLORREF color, int x, int y) const
{
	SetTextColor(backbufferDC, color);
	TextOut(backbufferDC, x, y, text.c_str(), text.size());
}

void MemoryDrawer::DrawWinGame()
{
	SetGameoverFontSettings();
	std::wstring result(_T("YOU WON"));
	DrawString(result, RGB(255, 0, 0), windowWidth / 2 - 70, windowHeight / 2);
}

void MemoryDrawer::DrawLoseGame()
{
	SetGameoverFontSettings();
	std::wstring result(_T("YOU LOSE"));
	DrawString(result, RGB(255, 0, 0), windowWidth / 2 - 70, windowHeight / 2);
}

void MemoryDrawer::DrawRect(RECT rect, HBRUSH brush)
{
	FillRect(backbufferDC, &rect, brush);
}

void MemoryDrawer::DrawBackgroundRect(RECT rect, HBRUSH brush)
{
	FillRect(backgroundBufferDC, &rect, brush);
}

void MemoryDrawer::SetGameoverFontSettings() {
	SetBkMode(backbufferDC, TRANSPARENT);
	HFONT hFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	def_font =  SelectObject(backbufferDC, hFont);
}
