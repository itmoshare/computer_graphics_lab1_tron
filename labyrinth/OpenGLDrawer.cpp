#include "OpenGLDrawer.h"



GLYPHMETRICSFLOAT g_GlyphInfo[256];
UINT g_FontListID = 0;
void OpenGlDrawer::OnInitializeGraphice(HWND window, int windowWidth, int windowHeight)
{
	g_hWnd = window;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	//g_FontListID = CreateOpenGL3DFont("Impact", 0.4f);

	// Because we are doing 3D, we need to enable depth testing.  This allows it so
	// the polygons are drawn in the order they are seen.

	//glEnable(GL_DEPTH_TEST);							// Enable depth testing 

														// I am not sure how you would get the normals of each polygon of the text,
														// but if we at least turn on a light and enable lighting, it will roughly
														// calculate that stuff for us, though it won't be perfect.  

	//glEnable(GL_LIGHT0);								// Enable Default Light (Quick And Dirty)
//	glEnable(GL_LIGHTING);								// Enable Lighting

														// Once we turn on lighting, we have to enable color material or else it won't
														// color our objects.  You can change many properties of that color in regards to
														// the light with glColorMaterial().

	//glEnable(GL_COLOR_MATERIAL);						// Enable Coloring Of Material
}




void OpenGlDrawer::OnBeginGraphics()
{
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	//glOrtho(0.0f, windowWidth, windowHeight, 0, -100.0f, 100.0f);

	// Calculate The Aspect Ratio Of The Window
	// The parameters are:
	// (view angle, aspect ration of the width to the height, 
	//  the closest distance to the camera before it clips, 
	// FOV		// Ratio				//  the farthest distance before it stops drawing).
	gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, -100.0f, 150.0f);


	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	//gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);
	//gluLookAt(0, -4, 6, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	glOrtho(0.0f, windowWidth, windowHeight, 0, -100.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix
	glPushAttrib(GL_CURRENT_BIT);
	for(int i=0; i < backRects.size(); i++)
	{
		this->DrawRect(backRects[i], brashes[i]);
	}
	glPopAttrib();
	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	//glLoadIdentity();									// Reset The Projection Matrix

	//gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.0f, 150.0f);

	//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	//glLoadIdentity();									// Reset The Modelview Matrix
	//glRotatef(0.5, 0, 0, 1);
	
}

void OpenGlDrawer::OnEndGraphics()
{
	glPopMatrix();
	SwapBuffers(g_hDC);									// Swap the backbuffers to the foreground

}
LONG fourthX;
LONG fourthY;
void OpenGlDrawer::DrawGdi(GDIBitmap gdi, bool player)
{
	glPushMatrix();
	//glOrtho(0.0f, windowWidth, windowHeight, 0, -1.0f, 1.0f);
	

	fourthX = gdi.points[1].x + gdi.points[2].x - gdi.points[0].x;
	fourthY = gdi.points[1].y + gdi.points[2].y - gdi.points[0].y;

	glEnable(GL_TEXTURE_2D);

	if(player)
		glBindTexture(GL_TEXTURE_2D, g_Texture[0]);                                      // Bind To The Texture ID
	else
		glBindTexture(GL_TEXTURE_2D, g_Texture[1]);                                      // Bind To The Texture ID

	// Display a quad texture to the screen
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);	glVertex3f(fourthX, fourthY, 0.5);
	glTexCoord2f(0, 1); glVertex3f(gdi.points[1].x, gdi.points[1].y, 0.5);
	glTexCoord2f(1, 1); glVertex3f(gdi.points[0].x, gdi.points[0].y, 0.5);
	glTexCoord2f(1, 0); glVertex3f(gdi.points[2].x, gdi.points[2].y, 0.5);
	/*glTexCoord2f(0, 0);	glVertex3f(fourthX/windowWidth, fourthY / windowHeight, 0.5);
	glTexCoord2f(0, 1); glVertex3f(gdi.points[1].x / windowWidth, gdi.points[1].y / windowHeight, 0.5);
	glTexCoord2f(1, 1); glVertex3f(gdi.points[0].x / windowWidth, gdi.points[0].y / windowHeight, 0.5);
	glTexCoord2f(1, 0); glVertex3f(gdi.points[2].x / windowWidth, gdi.points[2].y / windowHeight, 0.5);*/
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void OpenGlDrawer::DrawString(const char * text) const
{
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glPushMatrix();
	//glOrtho(0.0f, windowWidth, windowHeight, 0, -1.0f, 1.0f);
//	glTranslatef(windowWidth / 2, windowHeight / 2, 0);
	glRasterPos2i(windowWidth / 2 - 100, windowHeight / 2);
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;
	int len = strlen(text);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, text[i]);
	}
	glPopMatrix();
	glPopAttrib();
}

void OpenGlDrawer::SizeOpenGLScreen(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero error
	{
		height = 1;										// Make the Height Equal One
	}

	glViewport(0, 0, width, height);						// Make our viewport the whole window
															// We could make the view smaller inside
															// Our window if we wanted too.
															// The glViewport takes (x, y, width, height)
															// This basically means, what are our drawing boundaries
	

}

void OpenGlDrawer::DrawWinGame()
{
	DrawString("YOU WIN");
}

void OpenGlDrawer::DrawLoseGame()
{
	DrawString("YOU LOSE");
}

void OpenGlDrawer::DrawRect(RECT rect, HBRUSH brush)
{
	
	LOGBRUSH lb;
	int r, g, b;
	if (GetObject(brush, sizeof(LOGBRUSH), (LPSTR)&lb))
	{
		r = GetRValue(lb.lbColor);
		g = GetGValue(lb.lbColor);
		b = GetBValue(lb.lbColor);
	}

	glPushMatrix();										// Create a matrix scope to not effect the rest
	glColor3ub(r, g, b);
	//glOrtho(0.0f, windowWidth, windowHeight, 0, -1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(rect.left, rect.top, 0);
	glVertex3f(rect.right, rect.top, 0);
	glVertex3f(rect.right, rect.bottom, 0);
	glVertex3f(rect.left, rect.bottom, 0);
	glEnd();

	glPopMatrix();
}

void OpenGlDrawer::DrawBackgroundRect(RECT rect, HBRUSH brush)
{
	backRects.push_back(rect);
	brashes.push_back(brush);
}

bool bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	int pixelformat;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
	pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
	pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
	pfd.cColorBits = 16;						// Here we use our #define for the color bits
	pfd.cDepthBits = 16;						// Depthbits is ignored for RGBA, but we do it anyway
	pfd.cAccumBits = 0;									// No special bitplanes needed
	pfd.cStencilBits = 0;								// We desire no stencil bits

														// This gets us a pixel format that best matches the one passed in from the device
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE)
	{
		MessageBox(NULL, TEXT("ChoosePixelFormat failed"), TEXT("Error"), MB_OK);
		return FALSE;
	}

	// This sets the pixel format that we extracted from above
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
	{
		MessageBox(NULL, TEXT("SetPixelFormat failed"), TEXT("Error"), MB_OK);
		return FALSE;
	}

	return TRUE;										// Return a success!
}

void OpenGlDrawer::InitializeOpenGL(LONG width, LONG height)
{
	g_hDC = GetDC(g_hWnd);								// This sets our global HDC
														// We don't free this hdc until the end of our program
	if (!bSetupPixelFormat(g_hDC))						// This sets our pixel format/information
		PostQuitMessage(0);							// If there's an error, quit

	g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
	wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use

	windowWidth = width;
	windowHeight = height;

	SizeOpenGLScreen(width, height);					// Setup the screen translations and viewport
}


bool CreateTexture(GLuint &textureID, BITMAP bitmap)                          // Creates Texture From A Bitmap File
{
	glGenTextures(1, &textureID);                                                 // Create The Texture
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                                        // Pixel Storage Mode (Word Alignment / 4 Bytes)

																				  // Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, textureID);                                      // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			  // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			  // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);

	return TRUE;                                                                  // Loading Was Successful
}

void OpenGlDrawer::CreateTextures(HBITMAP player, HBITMAP computer)
{   
	BITMAP playerBitmap;// Create The Texture
	GetObject(player, sizeof(playerBitmap), &playerBitmap);
	CreateTexture(g_Texture[0], playerBitmap);
	BITMAP cplayerBitmap;// Create The Texture
	GetObject(computer, sizeof(cplayerBitmap), &cplayerBitmap);
	CreateTexture(g_Texture[1], cplayerBitmap);
}

