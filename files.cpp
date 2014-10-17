#include "stdafx.h"
#include <stdlib.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sys/stat.h>
using namespace std;

extern "C" {
	int CCONV1(cdecl) h_fprintf(FILE* file, const char* fmt, va_list arglist) CCONV2(cdecl);
}

extern "C" {
	long file_length(char* f);
	void splitpath(
		const char* path,
		char* drive,
		char* dir,
		char* fname,
		char* ext
		);
}

int CCONV1(cdecl) h_fprintf(FILE* file, const char* fmt, va_list arglist) 
{ 
	//va_arg
	return vfprintf(file, fmt, arglist); 
}

long file_length(char *f)
{
    struct stat st;
    stat(f, &st);
    return st.st_size;
}

#include <libgen.h>

void splitpath (
   const char *path,  // Path Input
   char *drive,       // Drive     : Output
   char *dir,         // Directory : Output
   char *fname,       // Filename  : Output
   char *ext          // Extension : Output
)
{
	char* d = static_cast<char*>(malloc(strlen(path) + 1));
	char* n = static_cast<char*>(malloc(strlen(path) + 1));
	strcpy(d, path);
	strcpy(n, path);
	dirname(d);
	if (dir)
		strcpy(dir, d);
	n = basename(n);
	if (fname)
		strcpy(fname, n);
}
