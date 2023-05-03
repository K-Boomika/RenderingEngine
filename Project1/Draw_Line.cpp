#include "Draw_Line.h"

DrawLine::DrawLine()
{
}

DrawLine::DrawLine(Vector start, Vector end, Vector color)
{
	this->start = start;
	this->end = end;
	this->color = color;
}

DrawLine::~DrawLine()
{
}