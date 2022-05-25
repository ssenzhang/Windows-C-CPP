#include <windows.h>
#include "SYSMETS.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets1");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	// 窗口类设计
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// 窗口类注册
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	// 窗口创建，WM_CREATE
	hwnd = CreateWindow(szAppName,
		TEXT("Get System Metrics No. 1"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	// 窗口显示，WM_PAINT
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// 消息循环
	while (GetMessage(&msg, NULL, 0, 0))  // 消息队列去消息直到WM_QUIT
	{
		TranslateMessage(&msg);  // 将虚拟按键消息翻译成字符串消息
		DispatchMessage(&msg);   // 分发消息到窗口
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  cxChar, cxCaps, cyChar;
	HDC         hdc;
	int         i;
	PAINTSTRUCT ps;
	TCHAR       szBuffer[10];
	TEXTMETRIC  tm;

	switch (message)
	{
	case WM_CREATE: // CreateWindow
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);    // 系统默认字体尺寸
		cxChar = tm.tmAveCharWidth;  // 平均字符宽度

		// 大写字符平均宽度; 等宽字体等于cxChar, 变宽字体设为cxChar的1.5倍
		// tmPitchAndFamily低位决定字体是否是等宽字体：1表示变宽，0表示等宽
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2; 
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_PAINT:  // UpdateWindow
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{
			// 第一列，最长有20个字符
			TextOut(hdc, 0, cyChar * i,
				sysmetrics[i].szLabel,
				lstrlen(sysmetrics[i].szLabel));

			// 第二列
			TextOut(hdc, 22 * cxCaps, cyChar * i,
				sysmetrics[i].szDesc,
				lstrlen(sysmetrics[i].szDesc));

			// 第3列，数字右对齐. 40 * cxChar -- 第2、3列的宽度
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);  
			TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i, szBuffer,
				wsprintf(szBuffer, TEXT("%5d"),
					GetSystemMetrics(sysmetrics[i].iIndex)));
			// Restore
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}