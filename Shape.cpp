#include "Shape.h"


Shape::Shape(const POINT u, const POINT v, const LOGPEN & lp, const LOGBRUSH & lb)
{
	mPt0.x = u.x; 
	mPt0.y = u.y;
	mPt1.x = v.x;
	mPt1.y = v.y;

	mhPen = CreatePenIndirect(&lp);
	mhBrush = CreateBrushIndirect(&lb);
	mhOldPen = 0;
	mhOldBrush = 0;
}

Shape::~Shape() //Free any resources that were allocated (pen and brush)
{
	DeleteObject(mhPen);
	DeleteObject(mhBrush);
}

void Shape::SetStartPt(const POINT & p0)
{
	mPt0 = p0;
}

void Shape::SetEndPt(const POINT & p1)
{
	mPt1 = p1;
}
