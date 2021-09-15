#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>

// ***************************************************************
// printf("%s %d %c", "www", 1, 'c');
// < == >
// char temp[1024];
// sprintf(temp, "%s %d %c", "www", 1, 'c');
// puts(temp);
// ***************************************************************

int my_sprintf(char* szBuffer, const char* szFormat, ...)
{
	int iReturn;
	va_list pArgs;

	va_start(pArgs, szFormat);
	iReturn = vsprintf_s(szBuffer, 1024, szFormat, pArgs);
	va_end(pArgs);

	return iReturn;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TCHAR szText[1024] = { 0 };

	SYSTEMTIME sTime;
	GetSystemTime(&sTime);
	my_sprintf(szText, _T("Current time: %u-%u-%u %u:%u:%u"), sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond);

	MessageBox(NULL, szText, _T("Windows sprintf"), 0);

	return 0;
}