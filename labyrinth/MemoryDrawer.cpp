#include "MemoryDrawer.h"

void MemoryDrawer::OnInitializeGraphice(HWND window, int windowWidth, int windowHeight)
{
	this->window = window;
	this->defaultHdc = GetDC(window);
	RECT clientRect;
	GetClientRect(this->window, &clientRect);


	this->windowWidth = clientRect.right - clientRect.left;
	this->windowHeight = clientRect.bottom - clientRect.top;
	
	this->bitmapInfo = { 0 };
	this->bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	this->bitmapInfo.bmiHeader.biBitCount = 24;
	this->bitmapInfo.bmiHeader.biWidth = this->windowWidth;
	this->bitmapInfo.bmiHeader.biHeight = this->windowHeight;
	this->bitmapInfo.bmiHeader.biPlanes = 1;
	this->bitmapInfo.bmiHeader.biCompression = 0;

	bytes = new BYTE[3 * this->windowWidth * this->windowHeight];
	backBytes = new BYTE[3 * this->windowWidth * this->windowHeight];
	//clear
	for (int i = 0; i < this->bitmapInfo.bmiHeader.biWidth * this->bitmapInfo.bmiHeader.biHeight * 3; i++)
	{
		backBytes[i] = 0;
	}
}

void MemoryDrawer::CustomFillRect(RECT rect, int r, int g, int b)
{
	auto t = (((rect.right - rect.left) * 24 + 31) / 32) * 4;
	for (int lx = 0; lx < rect.right - rect.left; lx++)
	{
		for (int ly = 0; ly < rect.bottom - rect.top; ly++)
		{
			int ax = rect.left + lx;
			int ay = (windowHeight - rect.top) - ly;

			if (ax * 3 >= windowWidth * 3 ||
				ay * 3 >= windowHeight * 3 ||
				ax < 0 ||
				ay < 0)
				continue;
			int globalPixel = ax * 3 + ay * windowWidth * 3;

			backBytes[globalPixel] = b;
			backBytes[globalPixel + 1] = g;
			backBytes[globalPixel + 2] = r;
		}
	}
}

void MemoryDrawer::OnBeginGraphics()
{
	int countBits = this->bitmapInfo.bmiHeader.biWidth * this->bitmapInfo.bmiHeader.biHeight * 3;
	////clear in black
	for (int i = 0; i <  this->bitmapInfo.bmiHeader.biWidth * this->bitmapInfo.bmiHeader.biHeight * 3; i ++)
	{
		bytes[i] = backBytes[i];

	}
}

void MemoryDrawer::OnEndGraphics()
{
	SetDIBitsToDevice(defaultHdc, 0, 0, bitmapInfo.bmiHeader.biWidth, bitmapInfo.bmiHeader.biHeight, 0, 0, 0, bitmapInfo.bmiHeader.biHeight, &bytes[0], &bitmapInfo, DIB_RGB_COLORS);
}

void MemoryDrawer::DrawGdi(GDIBitmap gdi)
{
	BITMAPINFO tempBmp = { 0 };
	tempBmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	tempBmp.bmiHeader.biBitCount = 24;
	tempBmp.bmiHeader.biWidth = gdi.width;
	tempBmp.bmiHeader.biHeight = gdi.height;
	tempBmp.bmiHeader.biPlanes = 1;
	tempBmp.bmiHeader.biCompression = BI_RGB;

	auto t = ((gdi.width * 24 + 31) / 32) * 4;
	DWORD dwBmpSize = t * gdi.height;
	unsigned char *bits = new unsigned char[dwBmpSize];

	GetDIBits(defaultHdc, gdi.handle, 0, gdi.height, bits, &tempBmp, DIB_RGB_COLORS);
	
	for (int lx = 0; lx < tempBmp.bmiHeader.biWidth; lx++)
	{
		for (int ly = 0; ly < tempBmp.bmiHeader.biHeight; ly++)
		{
			int ax = gdi.points[0].x + lx;
			int ay = windowHeight - gdi.points[0].y - ly;

			if (ax * 3 >= windowWidth * 3 ||
				ay * 3 >= windowHeight * 3 ||
				ax < 0 ||
				ay < 0)
				continue;
			int globalPixel = ax * 3 + ay * windowWidth * 3;
			int imagePixel = lx * 3 + ly * t;

			bytes[globalPixel] = bits[imagePixel];
			bytes[globalPixel + 1] = bits[imagePixel + 1];
			bytes[globalPixel + 2] = bits[imagePixel + 2];

		}
	}
	delete bits;


	//SelectObject(bitmapDC, gdi.handle);
	//PlgBlt(backbufferDC, gdi.points, bitmapDC, 0, 0, gdi.width, gdi.height, NULL, 0, 0);
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
	LOGBRUSH lb;
	int r, g, b;
	if (GetObject(brush, sizeof(LOGBRUSH), (LPSTR)&lb))
	{
		r = GetRValue(lb.lbColor);
		g = GetGValue(lb.lbColor);
		b = GetBValue(lb.lbColor);
	}
	this->CustomFillRect(rect, r, g, b);
}

void MemoryDrawer::DrawBackgroundRect(RECT rect, HBRUSH brush)
{
	/*backRects.push_back(rect);
	brashes.push_back(brush);*/

	LOGBRUSH lb;
	int r, g, b;
	if (GetObject(brush, sizeof(LOGBRUSH), (LPSTR)&lb))
	{
		r = GetRValue(lb.lbColor);
		g = GetGValue(lb.lbColor);
		b = GetBValue(lb.lbColor);
	}
	this->CustomFillRect(rect, r, g, b);
}

void MemoryDrawer::SetGameoverFontSettings() {
	SetBkMode(backbufferDC, TRANSPARENT);
	HFONT hFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	def_font =  SelectObject(backbufferDC, hFont);
}
