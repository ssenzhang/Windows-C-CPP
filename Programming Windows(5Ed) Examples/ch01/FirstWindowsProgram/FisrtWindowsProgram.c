// ����Ŀ ������--ϵͳ--��ϵͳ--���� (/SUBSYSTEM:WINDOWS)
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MessageBox(NULL, TEXT("First Windows Program!"), TEXT("Hello Windows"), 0);
	return 0;
}