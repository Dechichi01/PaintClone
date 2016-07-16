#include "LineShape.h"


LineShape::LineShape(const POINT u, const POINT v, const LOGPEN & lp, const LOGBRUSH & lb) : Shape(u,v,lp,lb) { }

void LineShape::Draw(HDC hdc)
{
	mhOldPen = (HPEN)SelectObject(hdc, mhPen);//Returns a point to the PREVIOUSLY select item, it's good practice to store this item and select it back after drawing
	mhOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	//Draw Line
	MoveToEx(hdc, mPt0.x, mPt0.y, 0);
	LineTo(hdc, mPt1.x, mPt1.y);

	//Restore the old pen and brush
	SelectObject(hdc, mhOldPen);
	SelectObject(hdc, mhOldBrush);
}
