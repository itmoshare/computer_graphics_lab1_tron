#include "OpenGLDrawer.h"

void OpenGlDrawer::OnInitializeGraphice(HWND window, int windowWidth, int windowHeight)
{
	g_hWnd = window;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect
	programWalls = LoadShaders("VertexShader.vert", "FragmentShaderWalls.frag");
	programPlayers = LoadShaders("PalyerVertexShader.vert", "PlayerFragmentShader.frag");
	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programWalls, "MVP");
	MatrixID2 = glGetUniformLocation(programPlayers, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), (windowWidth * 1.0f) / (windowHeight * 1.0f) , 0.1f, 100.0f);
	
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(0, -2, 1.3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

										 //new
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Создадим 1 буфер и поместим в переменную vertexbuffer его идентификатор
	glGenBuffers(1, &vertexbuffer);

	glGenVertexArrays(1, &VertexArrayIDComputer);
	glBindVertexArray(VertexArrayIDComputer);
	glGenBuffers(1, &vertexBufferComputer);

	glGenVertexArrays(1, &VertexArrayIDPlayer);
	glBindVertexArray(VertexArrayIDPlayer);
	glGenBuffers(1, &vertexBufferPlayer);

	//colors
	glGenBuffers(1, &colorbuffer);

	//texture
	glGenBuffers(1, &uvBuffer);
	TextureID = glGetUniformLocation(programPlayers, "myTextureSampler");
}


void OpenGlDrawer::OnBeginGraphics()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i=0; i < backVertexes.size(); i++)
	{
		this->DrawBackgroundRectWithShader(backVertexes[i], backTypes[i]);
	}	
}

void OpenGlDrawer::DrawBackgroundRectWithShader(std::vector<GLfloat> vertexes, int type) {

	//GLfloat* vertex_buffe_data = &vertexes[0];
	GLfloat vertex_buffe_data[] = {
		vertexes[0], vertexes[1],vertexes[2],
		vertexes[3],vertexes[4],vertexes[5],
		vertexes[6],vertexes[7],vertexes[8],
		vertexes[9],vertexes[10],vertexes[11],
	};

	glUseProgram(programWalls);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// Указываем, что первым буфером атрибутов будут вершины
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Передадим информацию о вершинах в OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffe_data), vertex_buffe_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
		3,                  // Размер
		GL_FLOAT,           // Тип
		GL_FALSE,           // Указывает, что значения не нормализованы
		0,                  // Шаг
		(void*)0           // Смещение массива в буфере
	);

	static const GLfloat color_type[4][3] = {
		{0.0, 0.01, 0.15},
		{0.36, 0.47, 0.49 },
		{0.12, 0.53, 0.94},
		{0.99, 0.85, 0.0}
	};

	GLfloat color_buffer_data[] = {
		color_type[type][0],color_type[type][1],color_type[type][2],
		color_type[type][0],color_type[type][1],color_type[type][2],
		color_type[type][0],color_type[type][1],color_type[type][2],
		color_type[type][0],color_type[type][1],color_type[type][2]
	};

	//цвет
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                        // array buffer offset
	);

	glDrawArrays(GL_QUADS, 0, 4); 

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void OpenGlDrawer::InitPlayersBuffers(GDIBitmap gdiPlayer, GDIBitmap gdiComputer)
{
	LONG fourthX = gdiPlayer.points[1].x + gdiPlayer.points[2].x - gdiPlayer.points[0].x;
	LONG fourthY = gdiPlayer.points[1].y + gdiPlayer.points[2].y - gdiPlayer.points[0].y;

	std::vector<GLfloat> playerVertexBufferDataTemp3 = GetVertexBufferData({ gdiPlayer.points[0].x, gdiPlayer.points[0].y, fourthX, fourthY });
	const GLfloat vertex_buffe_data3[] = {
		playerVertexBufferDataTemp3[0], playerVertexBufferDataTemp3[1],playerVertexBufferDataTemp3[2],
		playerVertexBufferDataTemp3[3],playerVertexBufferDataTemp3[4],playerVertexBufferDataTemp3[5] ,
		playerVertexBufferDataTemp3[6],playerVertexBufferDataTemp3[7],playerVertexBufferDataTemp3[8] ,
		playerVertexBufferDataTemp3[9],playerVertexBufferDataTemp3[10],playerVertexBufferDataTemp3[11],
	};

	playerStartPos = glm::vec3(playerVertexBufferDataTemp3[0], playerVertexBufferDataTemp3[1], playerVertexBufferDataTemp3[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPlayer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffe_data3), vertex_buffe_data3, GL_STATIC_DRAW);

	LONG fourthX2 = gdiComputer.points[1].x + gdiComputer.points[2].x - gdiComputer.points[0].x;
	LONG fourthY2 = gdiComputer.points[1].y + gdiComputer.points[2].y - gdiComputer.points[0].y;

	std::vector<GLfloat> playerVertexBufferDataTemp2 = GetVertexBufferData({ fourthX2, fourthY2, gdiComputer.points[0].x, gdiComputer.points[0].y });
	const GLfloat vertex_buffe_data2[] = {
		playerVertexBufferDataTemp2[0], playerVertexBufferDataTemp2[1],playerVertexBufferDataTemp2[2],
		playerVertexBufferDataTemp2[3],playerVertexBufferDataTemp2[4],playerVertexBufferDataTemp2[5] ,
		playerVertexBufferDataTemp2[6],playerVertexBufferDataTemp2[7],playerVertexBufferDataTemp2[8] ,
		playerVertexBufferDataTemp2[9],playerVertexBufferDataTemp2[10],playerVertexBufferDataTemp2[11],
	};

	computerStartPos = glm::vec3(playerVertexBufferDataTemp2[0], playerVertexBufferDataTemp2[1], playerVertexBufferDataTemp2[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferComputer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffe_data2), vertex_buffe_data2, GL_STATIC_DRAW);

	//textureUv
	const GLfloat texCoordData[] =
	{
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0

	};

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordData), texCoordData, GL_STATIC_DRAW);
}

void OpenGlDrawer::DrawGdi(GDIBitmap gdi, bool player, Direction direction, int changeWnd)
{
	glUseProgram(this->programPlayers);
	
	GLfloat wh = windowWidth * 1.0f / 2.0f;
	GLfloat hh = windowHeight * 1.0f / 2.0f;
	GLfloat newX = (gdi.points[0].x - wh) / wh;
	GLfloat newY = (hh - gdi.points[0].y) / hh;

	glm::vec3 startpos = player ? playerStartPos : computerStartPos;
	glm::mat4 Model; //*glm::rotate(90.0f, glm::vec3(0, 1, 0));

	switch (direction)
	{
		case Direction::Up:
			Model = glm::translate(glm::vec3(newX, newY, 0) - startpos);
			break;
		case Direction::Down:
			Model = glm::translate(vec3(newX, newY, 0));
			Model = glm::rotate(Model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Model = glm::translate(Model, vec3(-newX, -newY, 0));
			Model = glm::translate(Model, glm::vec3(newX, newY, 0) - startpos);
			break;
		case Direction::Left:
			Model = glm::translate(vec3(newX, newY, 0));
			Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Model = glm::translate(Model, vec3(-newX, -newY, 0));
			Model = glm::translate(Model, glm::vec3(newX, newY, 0) - startpos);
				break;
		case Direction::Right:
			Model = glm::translate(vec3(newX, newY, 0));
			Model = glm::rotate(Model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Model = glm::translate(Model, vec3(-newX, -newY, 0));
			Model = glm::translate(Model, glm::vec3(newX, newY, 0) - startpos);
			break;
	}
	glm::mat4 MVP = Projection * View * Model;

	glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);


	int texutre_id;
	if (player)
		texutre_id = 0;
	else texutre_id = 1;
			
	//// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_Texture[texutre_id]);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);
	glEnableVertexAttribArray(0);
	
	if (player)
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPlayer);
	else
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferComputer);


	glVertexAttribPointer(
		0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                        // array buffer offset
	);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);

	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

std::vector<GLfloat> OpenGlDrawer::GetVertexBufferData(RECT rect)
{
	GLfloat wh = windowWidth * 1.0f / 2.0f;
	GLfloat hh = windowHeight * 1.0f / 2.0f;
	
	GLfloat left =(rect.left - wh) / wh;
	GLfloat top = (hh - rect.top) / hh;
	GLfloat right = (rect.right - wh) / wh;
	GLfloat bottom = (hh - rect.bottom) / hh;

	std::vector<GLfloat> g_vertex_buffer_data = {
		left, top, 0,
		left, bottom, 0,
		right, bottom, 0,
		right, top, 0,
	};
	return g_vertex_buffer_data;
}
void OpenGlDrawer::OnEndGraphics()
{
	SwapBuffers(g_hDC);									// Swap the backbuffers to the foreground
}



void OpenGlDrawer::SizeOpenGLScreen(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero error
	{
		height = 1;										// Make the Height Equal One
	}

}

void OpenGlDrawer::SetGameoverFontSettings() {
	SetBkMode(g_hDC, TRANSPARENT);
	HFONT hFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	def_font = SelectObject(g_hDC, hFont);
}

void OpenGlDrawer::DrawString(const std::wstring text, COLORREF color, int x, int y) const
{
	SetTextColor(g_hDC, color);
	TextOut(g_hDC, x, y, text.c_str(), text.size());
}

void OpenGlDrawer::DrawWinGame()
{
	SetGameoverFontSettings();
	std::wstring result(_T("YOU WON"));
	DrawString(result, RGB(255, 0, 0), windowWidth / 2 - 70, windowHeight / 2);
}

void OpenGlDrawer::DrawLoseGame()
{
	SetGameoverFontSettings();
	std::wstring result(_T("YOU LOSE"));
	DrawString(result, RGB(255, 0, 0), windowWidth / 2 - 70, windowHeight / 2);
}

void OpenGlDrawer::DrawRect(RECT rect, HBRUSH brush)
{
}

void OpenGlDrawer::DrawBackgroundRect(RECT rect, int type)
{
	std::vector<GLfloat> g_vertex_buffer_data = GetVertexBufferData(rect);
	
	backVertexes.push_back(g_vertex_buffer_data);
	backTypes.push_back(type);
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
	backbufferDC = CreateCompatibleDC(g_hDC);
														// We don't free this hdc until the end of our program
	if (!bSetupPixelFormat(g_hDC))						// This sets our pixel format/information
		PostQuitMessage(0);							// If there's an error, quit

	g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
	wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use

	windowWidth = width;
	windowHeight = height;

	glewInit();
}


bool CreateTexture(GLuint &textureID, BITMAP bitmap)                          // Creates Texture From A Bitmap File
{
	glGenTextures(1, &textureID);                                                 // Create The Texture
	//
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                                        // Pixel Storage Mode (Word Alignment / 4 Bytes)

	//																			  // Typical Texture Generation Using Data From The Bitmap
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

