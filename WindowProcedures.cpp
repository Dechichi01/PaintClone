#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Device context and PaintStruct for painting
	HDC hdc = 0;
	PAINTSTRUCT ps;

	POINT p0, p1;

	switch (msg)
	{
	case WM_CREATE: //Check default items
		CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
		return 0;
	case WM_COMMAND://A menu item was checked
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			DestroyWindow(ghMainWnd);
			return 0;
			//=============== Primitives ===============
		case ID_PRIMITIVE_LINE:
			//Check primitive line and uncheck current select primitive
			CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_LINE;
			return 0;
		case ID_PRIMITIVE_RECTANGLE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_RECTANGLE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_RECTANGLE;
			return 0;
		case ID_PRIMITIVE_ELLIPSE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_ELLIPSE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_ELLIPSE;
			return 0;
			//=============== Pen Colors ===============
		case ID_PENCOLOR_BLACK:
			CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_BLACK;
			gLogPen.lopnColor = BLACK;
			return 0;
		case ID_PENCOLOR_WHITE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_WHITE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_WHITE;
			gLogPen.lopnColor = WHITE;
			return 0;
		case ID_PENCOLOR_RED:
			CheckMenuItem(ghMenu, ID_PENCOLOR_RED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_RED;
			gLogPen.lopnColor = RED;
			return 0;
		case ID_PENCOLOR_GREEN:
			CheckMenuItem(ghMenu, ID_PENCOLOR_GREEN, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_GREEN;
			gLogPen.lopnColor = GREEN;
			return 0;
		case ID_PENCOLOR_BLUE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_BLUE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_BLUE;
			gLogPen.lopnColor = BLUE;
			return 0;
			//=============== Brush Colors ===============
		case ID_BRUSHCOLOR_BLACK:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
			gLogBrush.lbColor = BLACK;
			return 0;
		case ID_BRUSHCOLOR_WHITE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_WHITE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_WHITE;
			gLogBrush.lbColor = WHITE;
			return 0;
		case ID_BRUSHCOLOR_RED:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_RED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_RED;
			gLogBrush.lbColor = RED;
			return 0;
		case ID_BRUSHCOLOR_GREEN:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_GREEN, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_GREEN;
			gLogBrush.lbColor = GREEN;
			return 0;
		case ID_BRUSHCOLOR_BLUE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLUE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_BLUE;
			gLogBrush.lbColor = BLUE;
			return 0;
			//=============== Pen Styles ===============
		case ID_PENSTYLE_SOLID:
			CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_SOLID;
			gLogPen.lopnStyle = PS_SOLID;
			return 0;
		case ID_PENSTYLE_DOTTED:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DOTTED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DOTTED;
			gLogPen.lopnStyle = PS_DOT;
			return 0;
		case ID_PENSTYLE_DASHED:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DASHED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DASHED;
			gLogPen.lopnStyle = PS_DOT;
			return 0;
			//=============== Brush Styles ===============
		case ID_BRUSHSTYLE_SOLID:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;
			gLogBrush.lbStyle = BS_SOLID;
			return 0;
		case ID_BRUSHSTYLE_NULL:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_NULL, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_NULL;
			gLogBrush.lbStyle = BS_NULL;
			return 0;
		case ID_BRUSHSTYLE_DIAGONAL:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_DIAGONAL, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_DIAGONAL;
			gLogBrush.lbStyle = BS_HATCHED;
			gLogBrush.lbHatch = HS_BDIAGONAL;
			return 0;
		case ID_BRUSHSTYLE_CROSS:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_CROSS, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_CROSS;
			gLogBrush.lbStyle = BS_HATCHED;
			gLogBrush.lbHatch = HS_CROSS;
			return 0;
		}
	case WM_LBUTTONDOWN:
		//Capture the mouse, so we still have mouse input even if the mouse leaves the client area
		SetCapture(hWnd);
		gMouseDown = true;

		//Get the mouse coordinate points
		p0.x = LOWORD(lParam);
		p0.y = HIWORD(lParam);

		//We don't know the end point yet, so set it to zero
		p1.x = 0;
		p1.y = 0;

		//Create the shapre
		switch (gCurrPrimSel)
		{
		case ID_PRIMITIVE_LINE:
			gShape = new LineShape(p0, p1, gLogPen, gLogBrush);
			break;
		case ID_PRIMITIVE_RECTANGLE:
			gShape = new RectShape(p0, p1, gLogPen, gLogBrush);
			break;
		case ID_PRIMITIVE_ELLIPSE:
			gShape = new EllipseShape(p0, p1, gLogPen, gLogBrush);
			break;
		}

		return 0;

	case WM_MOUSEMOVE:
		if (gMouseDown)
		{
			p1.x = LOWORD(lParam);
			p1.y = HIWORD(lParam);

			gShape->SetEndPt(p1);
			//Repaint the window
			InvalidateRect(hWnd, 0, true);
		}
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();
		gMouseDown = false;

		p1.x = LOWORD(lParam);
		p1.y = HIWORD(lParam);
		gShape->SetEndPt(p1);

		gShapes.push_back(gShape);

		InvalidateRect(hWnd, 0, true);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (gMouseDown)
			gShape->Draw(hdc);
		for (int i = 0; i < gShapes.size(); i++)
			gShapes[i]->Draw(hdc);

		EndPaint(hWnd, &ps);
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	//Forward any other messages we didn't handle to the default window procedure
	return DefWindowProc(hWnd, msg, wParam, lParam);
}