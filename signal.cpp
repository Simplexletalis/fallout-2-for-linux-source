#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "interop_macros.h"

struct fo_jmp_buf {
	uint32 magic;
	int* real_buf;
	uint32 retaddr;
	uint32 ecx;
	uint32 edx;
	uint32 edi;
};
static int errorno;

extern "C" {
	void CCONV1(cdecl) h_fatal_runtime_error(const char* a, int b) CCONV2(cdecl);
	void CCONV1(cdecl) h_GNWSystemError(const char* msg) CCONV2(cdecl);
	int* CCONV1(cdecl) h_get_errno_ptr() CCONV2(cdecl);
	void CCONV1(cdecl) h_longjmp(struct fo_jmp_buf* buf, int value) CCONV2(cdecl);
	void CCONV1(cdecl) h_assert(int test, const char* msg, const char* file, int line) CCONV2(cdecl);

	void* setjmp_ptr=(void*)&setjmp;
}

void CCONV1(cdecl) h_fatal_runtime_error(const char* a, int b) {
	printf("%d: %s", b, a);
	abort();
}

void CCONV1(cdecl) h_GNWSystemError(const char* msg) {
	printf("%s", msg);
	abort();
}

int* CCONV1(cdecl) h_get_errno_ptr() { _get_errno(&errorno); return &errorno; }

void CCONV1(cdecl) h_longjmp(struct fo_jmp_buf* buf, int value) {
	if(buf->magic!=0x1bcd1234) {
		h_fatal_runtime_error("Trying to longjmp with an uninited buffer", 0);
	}
	longjmp(buf->real_buf, value);
}

void CCONV1(cdecl) h_assert(int test, const char* msg, const char* file, int line) {
	char buf[256];
	if(test) return;
	sprintf(buf, "Assertion fail: [%s] File: '%s' Line: %d", msg, file, line);
	h_GNWSystemError(buf);
}
