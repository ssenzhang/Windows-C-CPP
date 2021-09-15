#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int CDECL MessageBoxPrintf(TCHAR* szCaption, TCHAR* szFormat, ...)
{
	TCHAR szBuffer[1024];

	va_list pArgList;
	// The va_start macro (defined in STDARG.H) is usually equivalent to:
	// pArgList = (char*) &szFormat + sizeof(szFormat);

	va_start(pArgList, szFormat);

	// The last argument to wvsprintf points to the arguments
	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), sizeof(szBuffer) / sizeof(TCHAR), szFormat, pArgList);

	// The va_end macro just zeros out pArgList for no good reason
	va_end(pArgList);

	return MessageBox(NULL, szBuffer, szCaption, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR szCmdLine, int nCmdShow)
{
	int nCxScreen, nCyScreen;
	nCxScreen = GetSystemMetrics(SM_CXSCREEN);
	nCyScreen = GetSystemMetrics(SM_CYSCREEN);

	MessageBoxPrintf(_T("ScrnSize"), _T("The screen is %i pxiels wide by %i pixels hign."), nCxScreen, nCyScreen);

	return 0;
}