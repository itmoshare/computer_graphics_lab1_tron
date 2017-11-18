#ifndef GDIBITMAP_H
#define GDIBITMAP_H

#include <Windows.h>

struct GDIBitmap
{
	HBITMAP handle;
	int width;
	int height;
	POINT points[3];

	GDIBitmap()
		: handle(0)
	{
	}
};

#endif