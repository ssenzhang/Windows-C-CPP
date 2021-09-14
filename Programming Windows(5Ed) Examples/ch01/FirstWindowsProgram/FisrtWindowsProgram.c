// 空项目 链接器--系统--子系统--窗口 (/SUBSYSTEM:WINDOWS)
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MessageBox(NULL, TEXT("First Windows Program!"), TEXT("Hello Windows"), 0);
	return 0;
}