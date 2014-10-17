#include "stdafx.h"
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
#include "Events.h"
#include <stdio.h>

extern "C" int _cdecl entryPoint(int argc, char **argv);

extern "C"
{
	int* _focus;	// Window focus
	int* __51ede0;	// i don't know
	unsigned int* _window;	// window pointer

	void _gnw_main();
}

// cpp implamentation of entryPoint
int ePoint(int argc, char** argv)
{
	_focus = new int(1);
	__51ede0 = new int(1);
	unsigned int temp;
	asm("movl 56(%%esp), %0" : "=a"(temp));
	_window = new unsigned int(temp);
	asm("movl 48(%esp), %eax\n\t");
	asm("movl 52(%esp), %edx\n\t");
	_gnw_main();
}

int mymain(int argc, char **argv) {
	SdlInit();
	int result=ePoint(argc, argv);
	SdlExit();
	return result;
}

int _stdcall main(int argc, char** argv) {
	mymain(argc, argv);
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

//extern "C" int _stdcall OutputDebugStringA(const char *c);

extern "C" void _cdecl DebugPrintf(const char* fmt, va_list va) {
	char buf[256];
	vsprintf(buf, fmt, va);
	//OutputDebugStringA(buf);
}
