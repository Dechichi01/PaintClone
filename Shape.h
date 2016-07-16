#pragma once
#include<Windows.h>
class Shape
{
public:
	Shape(const POINT u, const POINT v, const LOGPEN& lp, const LOGBRUSH& lb);
	virtual ~Shape();

	void SetStartPt(const POINT& p0);
	void SetEndPt(const POINT& p1);

	virtual void Draw(HDC hdc);

	void operator=(Shape& rhs);

protected:
	POINT mPt0;
	POINT mPt1;
	HPEN mhPen;
	HBRUSH mhBrush;

	HPEN mhOldPen;
	HBRUSH mhOldBrush;
};

