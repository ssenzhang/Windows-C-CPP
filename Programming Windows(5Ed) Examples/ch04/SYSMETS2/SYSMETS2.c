#include <windows.h>
#include "..\SYSMETS1\SYSMETS.h"

// 效率差

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets2");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	// 窗口类设计
	wndclass.style = CS_HREDRAW | CS_VREDRAW;  // 滚动条时Paint
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
	hwnd = CreateWindow(szAppName, TEXT("Get System Metrics No. 2"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	// 窗口显示，WM_PAINT
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// 消息循环
	while (GetMessage(&msg, NULL, 0, 0))  // 消队列取消息，直到WM_QUIT
	{
		TranslateMessage(&msg);  // 翻译消息，虚拟键消息转换成字符消息
		DispatchMessage(&msg);   // 分发消息到窗口过程
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  cxChar, cxCaps, cyChar, cyClient;
	static int iVscrollPos; // 滑块当前位置
	HDC         hdc;
	int         i, y;
	PAINTSTRUCT ps;
	TCHAR       szBuffer[10];
	TEXTMETRIC  tm;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);

		SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);  // 滚动条范围
		SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);       // 滚动条位置
		return 0;

	case WM_SIZE:
		//cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollPos -= 1;  // 上移一行
			break;

		case SB_LINEDOWN:
			iVscrollPos += 1;  // 下移一行
			break;

		case SB_PAGEUP:
			iVscrollPos -= cyClient / cyChar; // 点击滑块上方空白处移动一个page
			break;

		case SB_PAGEDOWN:
			iVscrollPos += cyClient / cyChar; // 点击滑块下方空白处移动一个page
			break;

		case SB_THUMBPOSITION:
			iVscrollPos = HIWORD(wParam);  // 拖动滑块
			break;

		default:
			break;
		}

		iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));

		if (iVscrollPos != GetScrollPos(hwnd, SB_VERT))  // 获取当前滑块位置
		{
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
			// 并未在处理滚动条时直接绘制客户区，而是使客户区无效，将WM_PAINT置于消息队列统一绘制。
			InvalidateRect(hwnd, NULL, TRUE);  
			//UpdateWindow(hwnd);
			// WM_PAINT为低优先级消息，窗口可能需要等一会才收到此消息；
			// 所以会出现关闭对话框等待窗口重绘时屏幕留下空白的情况。
			// 如果客户区任何一部分无效，UpdateWindow将使窗口过程立刻收到WM_PAINT消息；
			// 如果整个客户区是有效的，UpdateWindow不会调用窗口过程，WM_PAINT将越过消息队列，直接由
			// Windows调用窗口过程，当窗口绘制完成后返回，UpdateWindow将控制权返回调用它的代码。
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{
			// i小于iVscrollPos时，输出到客户区外
			y = cyChar * (i - iVscrollPos);

			TextOut(hdc, 0, y,
				sysmetrics[i].szLabel,
				lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, 22 * cxCaps, y,
				sysmetrics[i].szDesc,
				lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer,
				wsprintf(szBuffer, TEXT("%5d"),
					GetSystemMetrics(sysmetrics[i].iIndex)));

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
