#ifndef __DrawLine__
#define __DrawLine__
#include "Vector.h"

class DrawLine
{
public:
	Vector start, end;
	Vector color;
public:
	DrawLine();
	DrawLine(Vector start, Vector end, Vector color);
	~DrawLine();
};

#endif
