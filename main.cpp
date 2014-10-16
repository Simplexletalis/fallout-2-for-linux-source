#include "stdafx.h"
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
#include "Events.h"
#include <stdio.h>

extern "C" int _cdecl entryPoint(int argc, char **argv, int hwnd);

int mymain(int argc, char **argv) {
	SdlInit();
	int result=entryPoint(argc, argv, 0);
	SdlExit();
	return result;
}

int _stdcall WinMain(uint32 hInstance, uint32 hPrevInstance, char* lpCmdLine, int nCmdShow) {
	mymain(1, &lpCmdLine);
}

#if 0
typedef int (_stdcall *inputFunc)(int,int,int,int);
extern "C" int _stdcall FakeDInputCreate(int a, int b, int c, int d) {
	HMODULE h=LoadLibraryA("dinput.dll");
	inputFunc func=(inputFunc)GetProcAddress(h, "DirectInputCreateA");
	return func(a,b,c,d);
}

typedef int (_stdcall *ddrawFunc)(int,int,int);
extern "C" int _stdcall FakeDDrawCreate(int a, int b, int c) {
	HMODULE h=LoadLibraryA("ddraw.dll");
	ddrawFunc func=(ddrawFunc)GetProcAddress(h, "DirectDrawCreate");
	return func(a,b,c);
}
#endif

extern "C" int _stdcall OutputDebugStringA(const char *c);

extern "C" void _cdecl DebugPrintf(const char* fmt, va_list va) {
	char buf[256];
	vsprintf(buf, fmt, va);
	OutputDebugStringA(buf);
}
