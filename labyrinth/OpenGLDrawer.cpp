#include "OpenGLDrawer.h"



GLYPHMETRICSFLOAT g_GlyphInfo[256];
UINT g_FontListID = 0;
void OpenGlDrawer::OnInitializeGraphice(HWND window, int windowWidth, int windowHeight)
{
	g_hWnd = window;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	g_FontListID = CreateOpenGL3DFont("Impact", 0.4f);

	// Because we are doing 3D, we need to enable depth testing.  This allows it so
	// the polygons are drawn in the order they are seen.

	//glEnable(GL_DEPTH_TEST);							// Enable depth testing 

														// I am not sure how you would get the normals of each polygon of the text,
														// but if we at least turn on a light and enable lighting, it will roughly
														// calculate that stuff for us, though it won't be perfect.  

	//glEnable(GL_LIGHT0);								// Enable Default Light (Quick And Dirty)
	//glEnable(GL_LIGHTING);								// Enable Lighting

														// Once we turn on lighting, we have to enable color material or else it won't
														// color our objects.  You can change many properties of that color in regards to
														// the light with glColorMaterial().

	//glEnable(GL_COLOR_MATERIAL);						// Enable Coloring Of Material
}


void glRotateForever(float angle, float x, float y, float z)
{
	static float rotation = 0.0f;						// Create and init a static variable

														// This function is designed to increase every frame to produce a continuous rotation.
														// Other objects can use this, but they can not change the rotation.  A more useful
														// function would allow you to pass in a reference to a float.

	rotation += angle;									// Every frame increase the rotation

	glRotatef(rotation, x, y, z);						// Rotate along the given axis
}


UINT OpenGlDrawer::CreateOpenGL3DFont(LPSTR strFontName, float extrude)
{
	UINT	fontListID = 0;								// This will hold the base ID for our display list
	HFONT	hFont;										// This will store the handle to our font

														// Here we generate the lists for each character we want to use.
														// This function then returns the base pointer, which will be 1 because
														// we haven't created any other lists.  If we generated another list after
														// this, the base pointer would be at 257 since the last one used was 256 (which is MAX_CHARS)
	fontListID = glGenLists(256);					// Generate the list for the font

														// Now we actually need to create the font.  We use a windows function called:
														// CreateFont() that returns a handle to a font (HFONT).  Our CreateOpenGLFont()
														// function allows us to pass in a name and height.  For simplistic reasons, I left
														// other options out, but feel free to add them to your function (like bold, italic, width..)
														// In the last tutorial we passed in a height, now we pass in a extrude value.  This
														// value basically makes the thickness in 3D.  The height parameter in CreateFont() doesn't
														// matter now, so it doesn't really matter what value we put in, it is just ignored by wglUseFontOutlines().

	hFont = CreateFont(0,								// This HEIGHT parameter is ignored by wglUseFontOutlines(). Use glScalef()
		0,								// The WIDTH (If we leave this zero it will pick the best width depending on the height)
		0,								// The angle of escapement
		0,								// The angle of orientation
		FW_BOLD,						// The font's weight (We want it bold)
		FALSE,							// Italic - We don't want italic
		FALSE,							// Underline - We don't want it underlined
		FALSE,							// Strikeout - We don't want it strikethrough
		ANSI_CHARSET,					// This is the type of character set
		OUT_TT_PRECIS,					// The Output Precision
		CLIP_DEFAULT_PRECIS,			// The Clipping Precision
		ANTIALIASED_QUALITY,			// The quality of the font - We want anitaliased fonts
		FF_DONTCARE | DEFAULT_PITCH,		// The family and pitch of the font.  We don't care.
		TEXT("Arial"));					// The font name (Like "Arial", "Courier", etc...)

										// Now that we have created a new font, we need to select that font into our global HDC.
										// We store the old font so we can select it back in when we are done to avoid memory leaks.
	hOldFont = (HFONT)SelectObject(g_hDC, hFont);

	// This is the function that creates all the magic.  This will actually output the
	// polygons into our display lists, which creates the outlined characters.
	// But first, we need to give it our desired information.
	// One new thing from the last tutorial is GLYPH's.  A glyph is a 3D character.
	// The GLYPHMETRICSFLOAT structure used holds information about that 3D character,
	// such as it's width and height in 3D units, as well as orientation within it's bounding box.
	// We can use this information if we want to find out how long our sentences are in actual
	// unit lengths, not pixel length.
	wglUseFontOutlines(g_hDC,							// This is the global HDC with the desired font selected.
		0,								// This is the starting ASCII value.
		256 - 1,					// This is the ending ASCII value to use (255 is the last).
		fontListID,						// This is the base pointer of our display list we wish to use.
		0,								// This is the deviation from a true outline (floating point)
		extrude,						// This is the extrude value, or in other words, the thickness in Z.
		WGL_FONT_POLYGONS,				// We specify here that we want it rendered in polygons rather than lines
		g_GlyphInfo);					// The address to the buffer that will hold the 3D font info for each character.

										/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	return fontListID;									// Return the ID to the display list to use later
}

void glDraw3DText(const char *strString, ...)
{
	char		strText[256];							// This will hold our text to display
	va_list		argumentPtr;							// This will hold the pointer to the argument list
	float		unitLength = 0.0f;						// This will store the length of the 3D Font in unit length

														// If you have never used a va_list, listen up.  Remember printf()?
														// or sprintf()?  Well, you can add unlimited arguments into the text like:
														// printf("My name is %s and I am %d years old!", strName, age);
														// Well, that is what va_list's do.  

														// First we need to check if there was even a string given
	if (strString == NULL)								// Check if a string was given
		return;											// Don't render anything then

														// First we need to parse the string for arguments given
														// To do this we pass in a va_list variable that is a pointer to the list of arguments.
														// Then we pass in the string that holds all of those arguments.
	va_start(argumentPtr, strString);					// Parse the arguments out of the string

														// Then we use a special version of sprintf() that takes a pointer to the argument list.
														// This then does the normal sprintf() functionality.
	vsprintf_s(strText, strString, argumentPtr);			// Now add the arguments into the full string

	va_end(argumentPtr);								// This resets and frees the pointer to the argument list.

														// Below we find out the total length of the characters in 3D units, then center them.

	for (int i = 0; i < (int)strlen(strText); i++)		// Go through all of the characters
	{
		// Here we go through each character and add up it's length.
		// It doesn't matter where the text if facing in 3D, it uses a 2D system
		// for the width and height of the characters.  So X is the width, Y is the height.
		unitLength += g_GlyphInfo[strText[i]].gmfCellIncX;
	}

	// Here we translate the text centered around the XYZ according to it's width.
	// Since we have the length, we can just divide it by 2, then subtract that from the X.
	// This will then center the text at that position, in the way of width.
	// Can you see how to center the height too if you wanted? (hint: .gmfCellIncY)
	//glTranslatef(0.0f - (unitLength / 2), 0.0f, 0.0f);

	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	// Now, before we set the list base, we need to save off the current one.
	glPushAttrib(GL_LIST_BIT);							// This saves the list base information

														// Then we want to set the list base to the font's list base, which should be 1 in our case.
														// That way when we call our display list it will start from the font's lists'.
	glListBase(g_FontListID);							// This sets the lists base

														// Now comes the actually rendering.  We pass in the length of the string,
														// then the data types (which are characters so its a UINT), then the actually char array.
														// This will then take the ASCII value of each character and associate it with a bitmap.
	glCallLists((int)strlen(strText), GL_UNSIGNED_BYTE, strText);

	glPopAttrib();										// Return the display list back to it's previous state
}


void OpenGlDrawer::OnBeginGraphics()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix

	glPushAttrib(GL_CURRENT_BIT);
	for(int i=0; i < backRects.size(); i++)
	{
		this->DrawRect(backRects[i], brashes[i]);
	}
	glPopAttrib();
}

void OpenGlDrawer::OnEndGraphics()
{
	SwapBuffers(g_hDC);									// Swap the backbuffers to the foreground

}
LONG fourthX;
LONG fourthY;
void OpenGlDrawer::DrawGdi(GDIBitmap gdi, bool player)
{
	glPushMatrix();
	glOrtho(0.0f, windowWidth, windowHeight, 0, -1.0f, 1.0f);
	

	fourthX = gdi.points[1].x + gdi.points[2].x - gdi.points[0].x;
	fourthY = gdi.points[1].y + gdi.points[2].y - gdi.points[0].y;

	glEnable(GL_TEXTURE_2D);

	if(player)
		glBindTexture(GL_TEXTURE_2D, g_Texture[0]);                                      // Bind To The Texture ID
	else
		glBindTexture(GL_TEXTURE_2D, g_Texture[1]);                                      // Bind To The Texture ID

	// Display a quad texture to the screen
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);	glVertex3f(fourthX, fourthY, 0);
	glTexCoord2f(0, 1); glVertex3f(gdi.points[1].x, gdi.points[1].y, 0);
	glTexCoord2f(1, 1); glVertex3f(gdi.points[0].x, gdi.points[0].y, 0);
	glTexCoord2f(1, 0); glVertex3f(gdi.points[2].x, gdi.points[2].y, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);


	/*glRasterPos2i(gdi.points[0].x, gdi.points[0].y + gdi.height);

	
	
	glDrawPixels(gdi.width, gdi.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, BM.bmBits);*/
	glPopMatrix();
}

void OpenGlDrawer::DrawString(const char * text) const
{
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glPushMatrix();
	glOrtho(0.0f, windowWidth, windowHeight, 0, -1.0f, 1.0f);
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

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  the closest distance to the camera before it clips, 
														// FOV		// Ratio				//  the farthest distance before it stops drawing).
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, .5f, 150.0f);
	

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
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
	glOrtho(0.0f, windowWidth, windowHeight, 0, -1.0f, 1.0f);
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

	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

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

