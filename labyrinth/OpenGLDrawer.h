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
#include <GL\glew.h>
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library
#include <glut.h>
// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


#include "WindowOptions.h"
#include "Bitmap.h"
#include "GDIBitmap.h"
#include "LoadShaders.h"
#include "Direction.h"
using namespace glm;

class OpenGlDrawer {
public:
	OpenGlDrawer() {};
	~OpenGlDrawer() {};
	void OnInitializeGraphice(HWND window, int windowWidth, int windowHeight);
	void OnBeginGraphics();
	void OnEndGraphics();
	void DrawGdi(GDIBitmap gdi, bool player, Direction direction);
	void DrawString(const char * text) const;

	void SizeOpenGLScreen(int width, int height);
	void OpenGlDrawer::DrawString(const std::wstring text, COLORREF color, int x, int y) const;
	void DrawWinGame();
	void DrawLoseGame();

	void DrawRect(RECT rect, HBRUSH brush);
	void DrawBackgroundRect(RECT rect, int type); //0-wall, 1-border


	void InitializeOpenGL(LONG width, LONG height);

	void CreateTextures(HBITMAP palyer, HBITMAP computerPlayer);

	int windowWidth;
	int windowHeight;


	UINT g_Texture[2];
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
	std::vector<int>  backTypes;
	std::vector<std::vector<GLfloat>> backVertexes;

	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLuint textureBuffer;
	GLuint uvBuffer;
	GLuint MatrixID;
	GLuint MatrixID2;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 MVP;
	glm::vec3 unp;
	glm::vec3 unp1;
	glm::vec3 unp2;
	glm::vec3 unp3;
	
	glm::mat4 trans;

	GLYPHMETRICSFLOAT g_GlyphInfo[256];
	UINT g_FontListID = 0;

	HWND  g_hWnd;										// This is the handle for the window
	RECT  g_rRect;										// This holds the window dimensions
	HDC   g_hDC;											// General HDC - (handle to device context)
	HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
	HINSTANCE g_hInstance;								// This holds our window hInstance

	GLuint texturePlayer;
	GLuint textureComputePlayer;
	//temp

	UINT CreateOpenGL3DFont(LPSTR strFontName, float extrude);
	HFONT hOldFont;
	//new
	GLuint VertexArrayID;

	GLuint programWalls;
	GLuint programPlayers;

	GLuint TextureID;

	void DrawBackgroundRectWithShader(std::vector<GLfloat> vertexes, int type);
	std::vector<GLfloat> GetVertexBufferData(RECT rect);
private:
	void SetGameoverFontSettings();

};
#endif