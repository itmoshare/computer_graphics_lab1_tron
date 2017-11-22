#ifndef OPENGLDRAWER_H
#define OPENGLDRAWER_H

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <tchar.h>
#include <cassert>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library


#include "WindowOptions.h"
#include "Bitmap.h"
#include "GDIBitmap.h"

class OpenGlDrawer {
public:
	OpenGlDrawer() {};
	~OpenGlDrawer() {};
	void OnInitializeGraphice(HWND window, int windowWidth, int windowHeight);
	void OnBeginGraphics();
	void OnEndGraphics();
	void DrawGdi(GDIBitmap gdi);
	void DrawString(const std::wstring text, COLORREF color, int x, int y) const;

	void SizeOpenGLScreen(int width, int height);

	void DrawWinGame();
	void DrawLoseGame();

	void DrawRect(RECT rect, HBRUSH brush);
	void DrawBackgroundRect(RECT rect, HBRUSH brush);


	void InitializeOpenGL(LONG width, LONG height);

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

	std::vector<RECT> backRects;
	std::vector<HBRUSH>  brashes;


	HWND  g_hWnd;										// This is the handle for the window
	RECT  g_rRect;										// This holds the window dimensions
	HDC   g_hDC;											// General HDC - (handle to device context)
	HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
	HINSTANCE g_hInstance;								// This holds our window hInstance



	//temp

	UINT CreateOpenGL3DFont(LPSTR strFontName, float extrude);
	HFONT hOldFont;

private:
	void SetGameoverFontSettings();

};
#endif