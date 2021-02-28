#include "stdafx.h"
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
#include "files.h"
#include "events.h"
#include <stdio.h>
#include "zlib/zlib.h"


extern "C" int _cdecl entryPoint(int argc, char **argv);

extern "C"
{
	int* _focus;	// Window focus
	int* __51ede0;	// i don't know
//	unsigned int* _window;	// window pointer

	void _gnw_main();
}

// cpp implamentation of entryPoint
int ePoint(int argc, char** argv)
{
	_focus = new int(1);
	__51ede0 = new int(1);
//	unsigned int temp;
//	asm("movl 56(%%esp), %0" : "=a"(temp));
//	_window = new unsigned int(temp);
	asm("movl 48(%esp), %eax\n\t");
	asm("movl 52(%esp), %edx\n\t");
	_gnw_main();
}


int main(int argc, char **argv) {
	updateWorkingDir();
	SdlInit();
        printf("zlib %s\n", zlibVersion());
	//int result=ePoint(argc, argv);
	int result=entryPoint(argc, argv);
	SdlExit();
	return result;
}

extern "C" void _cdecl DebugPrintf(const char* fmt, va_list va) {
	vprintf(fmt, va);
        fflush(stdout);
}
