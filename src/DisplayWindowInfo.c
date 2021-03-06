//
//  DisplayWindowInfo.c
//  Copyright (c) 2002 by J Brown
//  Freeware
//
//  void SetWindowInfo(HWND hwnd)
//
//  Fill the window-tab-pane with list of child+siblings
//

#include "WinSpy.h"

#include "resource.h"
#include "Utils.h"

static BOOL CALLBACK ChildWindowProc(HWND hwnd, LPARAM lParam)
{
	TCHAR  ach[256];
	TCHAR  cname[256];
	TCHAR  wname[256];
	LVITEM lvitem;

	//only display 1st generation (1-deep) children -
	//(don't display child windows of child windows)
	if (GetRealParent(hwnd) == spy_hCurWnd)
	{
		GetClassName(hwnd, cname, ARRAYSIZE(cname));
		GetWindowText(hwnd, wname, ARRAYSIZE(wname));
		_stprintf_s(ach, ARRAYSIZE(ach), szHexFmt, (UINT)(UINT_PTR)hwnd);

		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
		lvitem.iSubItem = 0;
		lvitem.pszText = ach;
		lvitem.iItem = 0;
		lvitem.state = 0;
		lvitem.stateMask = 0;
		lvitem.iImage = 0;

		ListView_InsertItem((HWND)lParam, &lvitem);
		ListView_SetItemText((HWND)lParam, 0, 1, cname);
		ListView_SetItemText((HWND)lParam, 0, 2, wname);
	}
	return TRUE;
}

static BOOL CALLBACK SiblingWindowProc(HWND hwnd, LPARAM lParam)
{
	TCHAR  ach[256];
	TCHAR  cname[256];
	TCHAR  wname[256];
	LVITEM lvitem;

	//sibling windows must share the same parent
	if (spy_hCurWnd != hwnd && GetRealParent(hwnd) == GetRealParent(spy_hCurWnd))
	{
		GetClassName(hwnd, cname, ARRAYSIZE(cname));
		GetWindowText(hwnd, wname, ARRAYSIZE(wname));
		_stprintf_s(ach, ARRAYSIZE(ach), szHexFmt, (UINT)(UINT_PTR)hwnd);

		lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
		lvitem.iSubItem = 0;
		lvitem.pszText = ach;
		lvitem.iItem = 0;
		lvitem.state = 0;
		lvitem.stateMask = 0;
		lvitem.iImage = 0;

		ListView_InsertItem((HWND)lParam, &lvitem);
		ListView_SetItemText((HWND)lParam, 0, 1, cname);
		ListView_SetItemText((HWND)lParam, 0, 2, wname);
	}

	return TRUE;
}

//
//  Get a list of all Child + Siblings for the specified window -
//  Update the Windows tab accordingly
//
void SetWindowInfo(HWND hwnd)
{
	HWND hParentWnd = NULL;
	TCHAR ach[10];

	*ach = 0;

	HWND hwndList1 = GetDlgItem(WinSpyTab[WINDOW_TAB].hwnd, IDC_LIST1);
	HWND hwndList2 = GetDlgItem(WinSpyTab[WINDOW_TAB].hwnd, IDC_LIST2);

	ListView_DeleteAllItems(hwndList1);
	ListView_DeleteAllItems(hwndList2);


	// Get all children of the window
	if (hwnd)
	{
		EnumChildWindows(hwnd, ChildWindowProc, (LPARAM)hwndList1);

		// Get children of its PARENT (i.e, its siblings!)
		hParentWnd = GetRealParent(hwnd);
		if (hParentWnd)
			EnumChildWindows(hParentWnd, SiblingWindowProc, (LPARAM)hwndList2);
	}

	// Set the Parent hyperlink
	if (hwnd)
	{
		_stprintf_s(ach, ARRAYSIZE(ach), szHexFmt, (UINT)(UINT_PTR)hParentWnd);
	}
	SetDlgItemText(WinSpyTab[WINDOW_TAB].hwnd, IDC_PARENT, ach);

	// Set the Owner hyperlink
	if (hwnd)
	{
		_stprintf_s(ach, ARRAYSIZE(ach), szHexFmt, (UINT)(UINT_PTR)GetWindow(hwnd, GW_OWNER));
	}
	SetDlgItemText(WinSpyTab[WINDOW_TAB].hwnd, IDC_OWNER, ach);
}
