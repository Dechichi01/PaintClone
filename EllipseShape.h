#pragma once
#include "Shape.h"
class EllipseShape :
	public Shape
{
public:
	EllipseShape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);
	void Draw(HDC hdc);
};

