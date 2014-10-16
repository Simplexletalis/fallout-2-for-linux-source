#include "stdafx.h"
#include <stdlib.h>
#include "interop_macros.h"

typedef void (CCONV1(cdecl) *atexit_f)(void) CCONV2(cdecl);

extern "C" {
	int CCONV1(cdecl) h_atexit(atexit_f f) CCONV2(cdecl);
}

int CCONV1(cdecl) h_atexit(atexit_f f) { atexit(f); return 0; }
