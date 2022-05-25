#include <windows.h>
#include "..\SYSMETS1\SYSMETS.h"

// Ч�ʲ�

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets2");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	// ���������
	wndclass.style = CS_HREDRAW | CS_VREDRAW;  // ������ʱPaint
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// ������ע��
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	// ���ڴ�����WM_CREATE
	hwnd = CreateWindow(szAppName, TEXT("Get System Metrics No. 2"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	// ������ʾ��WM_PAINT
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// ��Ϣѭ��
	while (GetMessage(&msg, NULL, 0, 0))  // ������ȡ��Ϣ��ֱ��WM_QUIT
	{
		TranslateMessage(&msg);  // ������Ϣ���������Ϣת�����ַ���Ϣ
		DispatchMessage(&msg);   // �ַ���Ϣ�����ڹ���
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  cxChar, cxCaps, cyChar, cyClient;
	static int iVscrollPos; // ���鵱ǰλ��
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

		SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);  // ��������Χ
		SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);       // ������λ��
		return 0;

	case WM_SIZE:
		//cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollPos -= 1;  // ����һ��
			break;

		case SB_LINEDOWN:
			iVscrollPos += 1;  // ����һ��
			break;

		case SB_PAGEUP:
			iVscrollPos -= cyClient / cyChar; // ��������Ϸ��հ״��ƶ�һ��page
			break;

		case SB_PAGEDOWN:
			iVscrollPos += cyClient / cyChar; // ��������·��հ״��ƶ�һ��page
			break;

		case SB_THUMBPOSITION:
			iVscrollPos = HIWORD(wParam);  // �϶�����
			break;

		default:
			break;
		}

		iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));

		if (iVscrollPos != GetScrollPos(hwnd, SB_VERT))  // ��ȡ��ǰ����λ��
		{
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
			// ��δ�ڴ��������ʱֱ�ӻ��ƿͻ���������ʹ�ͻ�����Ч����WM_PAINT������Ϣ����ͳһ���ơ�
			InvalidateRect(hwnd, NULL, TRUE);  
			//UpdateWindow(hwnd);
			// WM_PAINTΪ�����ȼ���Ϣ�����ڿ�����Ҫ��һ����յ�����Ϣ��
			// ���Ի���ֹرնԻ���ȴ������ػ�ʱ��Ļ���¿հ׵������
			// ����ͻ����κ�һ������Ч��UpdateWindow��ʹ���ڹ��������յ�WM_PAINT��Ϣ��
			// ��������ͻ�������Ч�ģ�UpdateWindow������ô��ڹ��̣�WM_PAINT��Խ����Ϣ���У�ֱ����
			// Windows���ô��ڹ��̣������ڻ�����ɺ󷵻أ�UpdateWindow������Ȩ���ص������Ĵ��롣
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{
			// iС��iVscrollPosʱ��������ͻ�����
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
