#pragma once
#include "Shape.h"
class RectShape :
	public Shape
{
public:
	RectShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);
	void Draw(HDC hdc);
};

