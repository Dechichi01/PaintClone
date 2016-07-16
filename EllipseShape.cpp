#include "EllipseShape.h"


EllipseShape::EllipseShape(const POINT u, const POINT v, const LOGPEN & lp, const LOGBRUSH & lb) : Shape(u, v, lp, lb) { }

void EllipseShape::Draw(HDC hdc)
{
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	//Draw rectangle
	Ellipse(hdc, mPt0.x, mPt0.y, mPt1.x, mPt1.y);

	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}
