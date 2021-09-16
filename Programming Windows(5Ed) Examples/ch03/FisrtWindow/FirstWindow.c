// WinMain.cpp

/** First Microsoft Foundation Class Program
*********visual studio工程配置*******
> 创建C++空项目 链接器-系统-子系统 设置为窗口 (/SUBSYSTEM:WINDOWS)    
-- LINK2019 无法解析的外部符号,该符号在函数"int _cdecl invoke_main(void)"(?invoke_main@@YAHXZ)中引用
-- 控制台 (/SUBSYSTEM:CONSOLE)的入口函数为int main(void), 在链接时无法解析入口函数所以报错

> 注意以下几项的值：
> 项目-属性-常规 目标文件扩展名为.exe，配置类型为应用程序exe，使用标准Windows库否则链接出现无解析符号，字符集可以设置为UNICODE
> C/C++-语言-将WChar_t视为内置类型  wchar_t和UNICODE对应
> 链接器-系统-子系统 设置为窗口 (/SUBSYSTEM:WINDOWS)
************************/
#include <windows.h>  //底层实现窗口的头文件
#include <tchar.h>

#pragma comment(lib, "winmm")  // Winmm.lib

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//程序入口函数
//WINAPI 参数传递顺序：四个参数从右到左入栈，并在函数返回前清空堆栈
int WINAPI WinMain(
	HINSTANCE hInstance,       // 应用程序实例句柄
	HINSTANCE hPrevInstance,   // 上一个应用程序实例句柄，在Win32环境下一般为NULL
	LPSTR lpszCmdLine,         // char* argv	[]
	int nCmdShow)              // 显示命令： 最大化，最小化，default...
{
	// 1.设计窗口
	WNDCLASS wndclass;
	wndclass.style = /*0*/CS_HREDRAW | CS_VREDRAW;                // Class style 显示风格 0--default
	wndclass.cbClsExtra = 0;                                      // Class extra bytes 类的额外的内存	
	wndclass.cbWndExtra = 0;                                      // Window extra bytes 窗口额外的内存
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  // Background color 设置背景
	// wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.hCursor = LoadCursor(NULL, IDC_HAND);                // Cursor handle 设置光标，如果第一个参数为NULL代表使用系统提供的光标
	wndclass.hIcon = LoadIcon(NULL, IDI_WINLOGO/*IDI_ERROR*/);    // Icon handle 左上角图标,，如果第一个参数为NULL使用系统默认图标
	wndclass.hInstance = hInstance;                               // Instance handle 应用程序实例句柄，传入WinMain中的形参即可														
	wndclass.lpfnWndProc = (WNDPROC)WndProc;                      // Window procedure address 回调函数，窗口过程，第6步使用													
	wndclass.lpszClassName = TEXT("MyWndClass");                  // WNDCLASS name 指定窗口类名称
	wndclass.lpszMenuName = NULL;                                 // Menu name 菜单名称

    // 2.注册窗口类
	// 窗口类定义了窗口的重要特性，如窗口过程地址、默认背景色以及图标等；这些属性通过一个WNDCALSS(wc)结构体的字段值定义，随后传给RegisterClass
	// 当应用程序生成一个窗口时，必须指定一个窗口类，在该类能被使用之前，必须先对其进行注册；RegisterClass在程序开始即被调用
	// WNDCLASS类型窗口类与C++中的窗口类不一样，WNDCLASS为RegisterClass注册的类，C++中的窗口类指MFC的CWnd类派生的类
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requres Windows NT!"),
			TEXT("MyWndClass"), MB_ICONERROR);
		return 0;
	}

	// 3.创建窗口	
	// 一旦WNDCLASS被注册，WinMain将调用CreateWindow生成应用程序的窗口
	// 此函数11个参数, 无重载方式
	HWND hwnd = CreateWindow(
		TEXT("MyWndClass"),            // WNDCLASS name 窗口类名
		TEXT("My First MFC Dialog"),   // Window title 窗口标题名
		WS_OVERLAPPEDWINDOW,           // Window style 风格
		CW_USEDEFAULT,                 // Horizontal position 显示坐标 x值
		CW_USEDEFAULT,                 // Vertical position 显示坐标 y值
		CW_USEDEFAULT,                 // Initial width 宽
		CW_USEDEFAULT,                 // Initial Height 高
		HWND_DESKTOP/*NULL*/,          // Handle of parent window 父窗口
		NULL,                          // Menu handle 菜单
		hInstance,                     // Application's instance handle  实例句柄hInstance
		NULL                           // Window-creation data
	);

	// 4.显示刷新
	// 由于生成窗口时并未使用WS_VISIBLE, CreateWindow生成的窗口在屏幕上最初是不可见的；
	// 如果使用WS_VISIBLE则应该在CreateWindow中与WS_OVERLAPPEDWINDOW结合应用
	// 以下两个函数可以使窗口可见，WM_PAINT消息处理程序立刻被调用
	ShowWindow(hwnd, /*nCmdShow*/SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	// 5.通过消息循环取消息
	// 为了检索并调度消息，WinMain执行一个简单的反复调用GetMessage、TranslateMessage、
	// 为了检索并调度消息，WinMain执行一个简单的反复调用GetMessage、TranslateMessage、DispatchMessage三个API函数的while循环语句
	// GetMessage检查消息队列，如果某个消息是有效的，则将它从队列中删除并复制到msg，否则GetMessage将停留在消息队列上直到消息有效
	// TranslateMessage将一个指示字符键的键盘消息转换为更容易使用的WM_CHAR消息
	// DispatchMessage将消息发送给窗口过程
	// 消息循环一直执行到GetMessage返回值为0时结束，而此情形只有在WM_QUIT消息从消息队列中被检索到时才发生；这时WinMain结束，程序终止运行

	// typedef struct tagMSG {
	//	 HWND hwnd;          //主窗口句柄
	//	 UINT message;       //具体消息名称
	//	 WPARAM wParam;      //附加消息 键盘消息
	//	 LPARAM lParam;      //附加消息 鼠标消息
	//	 DWORD time;         //消息产生时间 操作系统反出 消息被放置在消息队列中的时间？
	//	 POINT pt;	         //附加消息 鼠标消息 坐标点消息 x y
	// }MSG;
	MSG msg;
	/**
	_Out_ LPMSG lpMsg,            消息
	_In_opt_ HWND hWnd,           捕获窗口 NULL为捕获所有窗口消息
	_In_ UINT wMsgFilterMin,      最小和最大过滤的消息，一般为0，代表捕获所有消息
	_In_ UINT wMsgFilterMax);
	*/
	while (GetMessage(&msg, NULL, 0, 0))      // 消息队列取消息
	{
		TranslateMessage(&msg);               // 翻译消息，例如组合键消息(CTRL+C)
		DispatchMessage(&msg);                // 分发消息到窗口过程
	}

	// 6.处理消息(窗口过程)

	// return 0;
	return (int)msg.wParam;
}

// CALLBACK 参数传递顺序：四个参数从右到左入栈，并在函数返回前清空堆栈
LRESULT CALLBACK WndProc(
	HWND hwnd,         // 消息所属窗口句柄
	UINT message,      // 消息 WM_XXXX
	WPARAM wParam,     // 键盘附加消息
	LPARAM lParam)     // 鼠标附加消息
{
	TCHAR buffer[256] = { 0 };
	TCHAR sztemp[256] = { 0 };
	PAINTSTRUCT ps;
	HDC hdc;
	POINT pt;

	switch (message)
	{
	case WM_CREATE:
		//PlaySound(TEXT("SystemStart"), NULL, SND_FILENAME | SND_ASYNC);  // system start sound
		MessageBox(NULL, TEXT("The window is being created, please enjoy a piece of music!"), TEXT("WM_CREATE"), message);
		PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		return 0;
	case WM_PAINT:           // 绘图
		hdc = BeginPaint(hwnd, &ps);   	// 获取设备环境句柄
		TextOut(hdc, 0, 0, TEXT("Draw a ellipse:"), (int)_tcscnlen(TEXT("Draw a ellipse:"), 256));
		Ellipse(hdc, 0, 20, 200, 100);
		EndPaint(hwnd, &ps);            // 释放设备环境句柄
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);   // 发送WM_DESTROY消息，并不会结束进程
		return 0;
	case WM_DESTROY:
		// 当接收到一个WM_DESTROY消息时，顶层窗口必须调用PostQuitMessage函数，否则消息循环不会停止，程序也永远不会结束
		PostQuitMessage(0);   // 给消息队列发送WM_QUIT消息，使程序停止运行；执行后GetMessage为FALSE
		return 0;
	case WM_LBUTTONDOWN:      // 鼠标左键按下
		/**
		Do not use the LOWORD or HIWORD macros to extract the x- and y- coordinates of the cursor position,
		because these macros return incorrect results on systems with multiple monitors.
		Systems with multiple monitors can have negative x- and y- coordinates,
		and LOWORD and HIWORD treat the coordinates as unsigned quantities.
		*/
		// Just a Test, and it's not suggested
		pt.x = LOWORD(lParam); // GET_X_LPARAM(lParam); "Windowsx.h"
		pt.y = HIWORD(lParam); // GET_Y_LPARAM(lParam);

		// wsprintf ==>sprintf(MBCS) or swprintf(UNICODE) 
		memset(buffer, 0, sizeof(TCHAR) * 256);
		wsprintf(buffer, TEXT("POINT.X = %ld, POINT.Y = %ld"), pt.x, pt.y);
		MessageBox(hwnd, buffer, TEXT("WM_LBUTTONDOWN"), message);
		return 0;
	case WM_KEYDOWN:
		memset(buffer, 0, sizeof(TCHAR)*256);
		memset(sztemp, 0, sizeof(TCHAR)*256);
		GetKeyNameText((long)lParam, sztemp, 256);
		wsprintf(buffer, TEXT("The %s was pressed down!"), sztemp);
		MessageBox(hwnd, buffer, TEXT("WM_KEYDOWM"), message);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);  // Default return value
}

/** 匈牙利标记法
	如 clr--COLORREF sz--以0结尾的字符串 psz--指向以0结尾的字符串的指针 m_xxx---类的成员变量
*/
//Windows是一个事件驱动、基于消息的操作系统，消息是在系统中发生任何事情的核心