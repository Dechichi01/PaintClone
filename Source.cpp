#include <Windows.h>
#include <vector>
#include <string>
#include "resource.h"
#include "Shape.h"
#include "LineShape.h"
#include "RectShape.h"
#include "EllipseShape.h"
using namespace std;

//Globals ============================//
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);

HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;
HMENU ghMenu = 0;

HWND ghDlg = 0;
HWND ghRadio = 0;
HWND ghCombo = 0;

vector<Shape*> gShapes;
Shape* gShape = 0;

bool gMouseDown = false;
bool gCtrlDown = false;

int gCurrPrimSel = ID_PRIMITIVE_LINE;
int gCurrPenColSel = ID_PENCOLOR_BLACK;
int gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
int gCurrPenStyleSel = ID_PENSTYLE_SOLID;
int gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;

LOGPEN gLogPen;
LOGBRUSH gLogBrush;
//==========================//

//Functions ================//
int HandleMenu(HWND hWnd, WPARAM wParam);
//==========================//

//Step1: Define and implement the window procedure
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
		HandleMenu(hWnd, wParam);
		return 0;
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
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(ghMainWnd);
			break;
		case VK_CONTROL:
			gCtrlDown = true;
			break;
		case 0x5A:
			if (gCtrlDown && gShapes.size() != 0)
			{
				Shape* shape = gShapes[gShapes.size() - 1];
				gShapes.pop_back();
				delete shape;
				InvalidateRect(hWnd, 0, true);
			}
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_CONTROL:
			gCtrlDown = false;
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	//Forward any other messages we didn't handle to the default window procedure
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//=========MODAL DIALOG EXAMPLE======//
BOOL CALLBACK
AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		return true; //We use WM_INITDIALOG to set default values. We don't need to set any for a About BOX
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0/*return code*/);
			return true;
		}
	}
	return false;
}

//========NON-MODAL DIALOG EXAMPLE======//
BOOL CALLBACK
EditDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char msgText[256];

	static HWND hEditBox = 0;

	switch (msg)
	{
	case WM_INITDIALOG:
		//Controls are CHILD WINDOWS to the dialog they lie on. In order to operate a control we need a HANDLE to it.
		hEditBox = GetDlgItem(hDlg, IDC_MSGTEXT);
		//Set some default text
		SetWindowText(hEditBox, L"Enter a message here.");
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDB_MSG:
			GetWindowText(hEditBox, (LPWSTR) msgText, 256);
			MessageBox(0, (LPWSTR)msgText, L"MessageText", MB_OK);
			return true;
		}
		return true;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		return true;
	}
	return false;
}
//=============RADIO BUTTON EXAMPLE =========//
BOOL CALLBACK
RadioDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int classSelection = IDC_RADIO_FIGHTER;
	string classNames[4] = { "You selected the fighter", "You selected the Cleric", "You selected the Thief", "You selected the Wizard" };

	switch (msg)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO_FIGHTER, IDC_RADIO_WIZARD, IDC_RADIO_FIGHTER);
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO_FIGHTER:
		case IDC_RADIO_CLERIC:
		case IDC_RADIO_THIEF:
		case IDC_RADIO_WIZARD:

			CheckRadioButton(hDlg, IDC_RADIO_FIGHTER, IDC_RADIO_WIZARD, LOWORD(wParam));
			classSelection = LOWORD(wParam);
			return true;
		case IDOK:
			MessageBox(0, (LPWSTR) (classNames[classSelection - IDC_RADIO_FIGHTER].c_str()), L"Message", MB_OK);
			return true;
		}
		return true;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		return true;
	}
	return false;
}
//===========COMBO BOX EXAMPLE=============//
BOOL CALLBACK
ComboDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char msgText[256];
	//Handles to the combo box controls
	static HWND hComboBox = 0;
	static HWND hEditBox = 0;
	static HWND hAddButton = 0;

	int index = 0;
	switch (msg)
	{
	case WM_INITDIALOG:
		hComboBox = GetDlgItem(hDlg, IDC_COMBOBOX);
		hEditBox = GetDlgItem(hDlg, IDC_EDIT_MSG);
		hAddButton = GetDlgItem(hDlg, IDC_ADDBUTTON);

		SetWindowText(hEditBox, L"Type a text to add at the dialog box.");
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case CBN_SELENDOK://User selected a combo box item
			index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);//We send a message to the ComboBox WindowProcedure (windows makes one for you) requesting for the id of the selected item
			SendMessage(hComboBox, CB_GETLBTEXT, (WPARAM)index, (LPARAM)msgText);//Get the text at the index and put it on msgText
			MessageBox(0, (LPWSTR)msgText, L"Message", MB_OK);
			return true;
		}
		switch (LOWORD(wParam))
		{
		case IDC_ADDBUTTON://User pressed the add button
			GetWindowText(hEditBox, (LPWSTR)msgText, 256);
			if (strlen(msgText) > 0)
				SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)msgText);//Send a message to the combobox requesting to add a string
			return true;
		}
		return true;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		return true;
	}
	return false;
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	ghAppInst = hInstance;
	
	//Deefine de WNDCLASS
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"WNDCLASS";

	RegisterClass(&wc);

	ghMenu = LoadMenu(ghAppInst, MAKEINTRESOURCE(IDR_MENU1));
	ghMainWnd = CreateWindow(L"WNDCLASS", L"PaintClone", WS_OVERLAPPEDWINDOW, 200, 200, 640, 480, 0, ghMenu, ghAppInst, 0);

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"Create Window - Failed", 0, 0);
		return false;
	}

	//Make sure the window is visible
	ShowWindow(ghMainWnd, showCmd);
	UpdateWindow(ghMainWnd);

	//Enter the message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, 0, 0, 0)) //GetMessage will be FALSE if the message is a WM_QUIT
	{
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg) && /*will return true if the message is for a dialog, and fwd the msg to it*/
		    ghRadio == 0 || !IsDialogMessage(ghRadio, &msg) &&
			ghCombo == 0 || !IsDialogMessage(ghCombo, &msg))
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//When we're quitting
	for (int i = 0; i < gShapes.size(); i++)
	{
		delete gShapes[i];
	}
	//Return exit code to OS
	return (int)msg.wParam;

}

int HandleMenu(HWND hWnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_FILE_EXIT:
		DestroyWindow(ghMainWnd);
		return 0;
	case ID_FILE_ABOUT:
		DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
		return 0;
	case ID_FILE_EDITDIALOG:
		ghDlg = CreateDialog(ghAppInst, MAKEINTRESOURCE(IDD_MSGDLG), 0, EditDlgProc);
		return 0;
	case ID_FILE_RADIOBUTTONS:
		ghRadio = CreateDialog(ghAppInst, MAKEINTRESOURCE(IDD_RADIODLG), 0, RadioDlgProc);
		return 0;
	case ID_FILE_COMBOBOX:
		ghCombo = CreateDialog(ghAppInst, MAKEINTRESOURCE(IDD_COMBODLG), 0, ComboDlgProc);
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
}


