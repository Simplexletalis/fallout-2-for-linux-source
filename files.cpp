#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

extern "C" {
	int CCONV1(cdecl) h_fprintf(FILE* file, const char* fmt, char** arglist) CCONV2(cdecl);
}

int CCONV1(cdecl) h_fprintf(FILE* file, const char* fmt, char** arglist) { return vfprintf(file, fmt, *arglist); }
