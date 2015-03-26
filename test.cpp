#include "stdafx.h"
#include "LaoYangWnd.h"
#pragma comment(lib,"Msimg32.lib")//TransparentBlt库

LONG CALLBACK WinYangProc(HWND hwnd,UINT uMsg,
						  WPARAM wParam,LPARAM lParam);
CLaoYangWnd lyWnd;//窗口对象

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int nShowCmd)
{
	WNDCLASS wndcls;
	wndcls.cbClsExtra=0;
	wndcls.cbWndExtra=0;
	wndcls.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wndcls.hCursor=LoadCursor(NULL,IDC_CROSS);
	wndcls.hIcon=LoadIcon(NULL,0);
	wndcls.hInstance=hInstance;
	wndcls.lpfnWndProc=WinYangProc;
	wndcls.lpszClassName="LaoYang";
	wndcls.lpszMenuName=NULL;
	wndcls.style=CS_HREDRAW | CS_VREDRAW;
	
	WinMessage wm = {"LaoYang","八数码求解",WS_CAPTION|WS_SYSMENU,
		0,0,608,670,NULL,NULL,hInstance,NULL,SW_SHOWNORMAL};

	lyWnd.SetWndMessage(&wndcls);
	lyWnd.PreCreateWindow(&wm);
	char *Num[] = {"0.bmp","1.bmp","2.bmp","3.bmp",
	"4.bmp","5.bmp","6.bmp","7.bmp","8.bmp"};
	lyWnd.Initialize(Num);
	lyWnd.MessageLoop();
	
	return 0;
}

LONG CALLBACK WinYangProc(HWND hwnd,UINT uMsg,
						  WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_F1:
			lyWnd.LoadFile();
			break;
		case VK_F2:
			if(lyWnd.run == false)
			{
				lyWnd.SetSumWay();
				lyWnd.run = true;
			}
			break;
		case VK_UP:
			lyWnd.SetDrect(0);
			break;
		case VK_DOWN:
			lyWnd.SetDrect(1);
			break;
		case VK_LEFT:
			lyWnd.SetDrect(2);
			break;
		case VK_RIGHT:
			lyWnd.SetDrect(3);
			break;
		}
		break;
	case WM_CLOSE:
		if(IDYES==MessageBox(hwnd,"是否真的结束？","老杨提示",MB_YESNO))
		{
			DestroyWindow(hwnd);
			exit(0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}
