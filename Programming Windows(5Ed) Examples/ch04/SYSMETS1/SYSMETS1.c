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

	// ���������
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

	// ������ע��
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	// ���ڴ�����WM_CREATE
	hwnd = CreateWindow(szAppName,
		TEXT("Get System Metrics No. 1"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	// ������ʾ��WM_PAINT
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// ��Ϣѭ��
	while (GetMessage(&msg, NULL, 0, 0))  // ��Ϣ����ȥ��Ϣֱ��WM_QUIT
	{
		TranslateMessage(&msg);  // �����ⰴ����Ϣ������ַ�����Ϣ
		DispatchMessage(&msg);   // �ַ���Ϣ������
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

		GetTextMetrics(hdc, &tm);    // ϵͳĬ������ߴ�
		cxChar = tm.tmAveCharWidth;  // ƽ���ַ����

		// ��д�ַ�ƽ�����; �ȿ��������cxChar, ���������ΪcxChar��1.5��
		// tmPitchAndFamily��λ���������Ƿ��ǵȿ����壺1��ʾ���0��ʾ�ȿ�
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2; 
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_PAINT:  // UpdateWindow
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{
			// ��һ�У����20���ַ�
			TextOut(hdc, 0, cyChar * i,
				sysmetrics[i].szLabel,
				lstrlen(sysmetrics[i].szLabel));

			// �ڶ���
			TextOut(hdc, 22 * cxCaps, cyChar * i,
				sysmetrics[i].szDesc,
				lstrlen(sysmetrics[i].szDesc));

			// ��3�У������Ҷ���. 40 * cxChar -- ��2��3�еĿ��
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