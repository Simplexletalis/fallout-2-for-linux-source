#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NO_HOOKS

#include <Windows.h>
#include <stdlib.h>
#include "interop_macros.h"

#pragma pack(push,1)
struct dirent {
    char    d_dta[ 21 ];        /* disk transfer area */
    char    d_attr;             /* file's attribute */
    unsigned short int d_time;  /* file's time */
    unsigned short int d_date;  /* file's date */
    long    d_size;             /* file's size */
    char    d_name[ 255 + 1 ]; /* file's name */
    unsigned short d_ino;       /* serial number */
    char    d_first;            /* flag for 1st time */
};
#pragma pack(pop)

struct dirhandle_t {
	WIN32_FIND_DATAA data;
	HANDLE hfind;
	dirent entry;
	bool firstfind;
	bool active;
};

static dirhandle_t dirhandle;

//dir functions
extern "C" dirhandle_t* _cdecl opendir(const char* dir) {
	WIN32_FIND_DATAA data;
	HANDLE hfind;

	if(dirhandle.active) {
		OutputDebugStringA("Uhoh: two nested opendir's just happened.\n");
	}

	hfind=FindFirstFileA(dir, &data);
	if(hfind==INVALID_HANDLE_VALUE) return 0;

	dirhandle_t *handle=&dirhandle;

	handle->data=data;
	handle->hfind=hfind;
	handle->firstfind=true;
	handle->active=true;

	return handle;
}

extern "C" dirent* _cdecl readdir(dirhandle_t* handle) {
	if(handle->firstfind) {
		handle->firstfind=false;
	} else {
		if(!FindNextFileA(handle->hfind, &handle->data)) return 0;
	}
	strcpy(handle->entry.d_name, handle->data.cFileName);
	return &handle->entry;
}

extern "C" void _cdecl closedir(dirhandle_t* handle) {
	if(!handle) return;
	if(handle->active) {
		FindClose(handle->hfind);
		handle->active=false;
	}
}

#endif