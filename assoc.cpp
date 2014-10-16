#if 0
#define _CRT_SECURE_NO_WARNINGS
#define NO_HOOKS

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "interop_macros.h"

struct sConfigFile;

struct pair {
	char* key;
	union {
		sConfigFile* value;
		char* str;
	};
};

static char section[256];

struct sConfigFile {
	union {
		char* value;
		uint32 magic;
	};
	int count;
	int max_count;
	int subitem_size;
	int d10;
	int d14;
	int d18;
	int d1c;
	int d20;
	pair* contents;
};

static int _stdcall assoc_init(sConfigFile* file, int a, int b, int c) {
	file->count=0;
	file->max_count=a;
	file->subitem_size=b;
	file->d10=c;
	file->d14=c;
	file->d18=c;
	file->d1c=c;
	if(c) file->d20=c;
	if(a) {
		file->contents=(pair*)malloc(sizeof(pair)*a);
		if(!file->contents) return -1;
	} else {
		file->contents=0;
	}
	file->magic=0xFEBAFEBA;
	return 0;
}
func _4d9ba8() { func4(assoc_init); }
static int _stdcall assoc_find(sConfigFile* file, const char* key, int* result) {
	if(file->magic!=0xFEBAFEBA) return -1;
	*result=0;
	if(file->count==0) return -1;
	int h=file->count-1;
	int l=0;
	while(true) {
		int m = (h+l)/2;
		int i=_stricmp(key, file->contents[m].key);
		if(!i) {
			*result=m;
			return 0;
		}
		if(h<=l) {
			*result=m;
			return -1;
		}
		if(i>0) l=m+1;
		else h=m-1;
	}
}
func _4d9cc4() { func3(assoc_find); }
static int _stdcall assoc_search(sConfigFile* file, const char* key) {
	if(file->magic!=0xFEBAFEBA) return -1;
	int d;
	if(assoc_find(file, key, &d)) return -1;
	return d;
}
func _4d9d5c() { func2(assoc_search); }
static int _stdcall assoc_resize(sConfigFile* file, int size) {
	if(file->magic!=0xFEBAFEBA) return -1;
	if(size<file->count) return -1;
	void* mem=realloc(file->contents, sizeof(pair)*size);
	if(!mem) return -1;
	file->contents=(pair*)mem;
	file->max_count=size;
	return 0;
}
func _4d9c0c() { func2(assoc_resize); }
static int _stdcall assoc_free(sConfigFile* file) {
	if(file->magic!=0xFEBAFEBA) return -1;
	for(int i=0;i<file->count;i++) {
		if(file->contents[i].key) free(file->contents[i].key);
		if(file->contents[i].value) free(file->contents[i].value);
	}
	free(file->contents);
	memset(file, 0, sizeof(sConfigFile));
	return 0;
}
func _4d9c48() { func1(assoc_free); }
static int _stdcall assoc_insert(sConfigFile* file, const char* key, void* value) {
	if(file->magic!=0xFEBAFEBA) return -1;
	int d;
	if(!assoc_find(file, key, &d)) return -1;
	if(file->count==file->max_count) {
		if(assoc_resize(file, 2*(file->count+1))==-1) return -1;
	}
	char* c=_strdup(key);
	char*mem=0;
	if(value&&file->subitem_size) {
		mem=(char*)malloc(file->subitem_size);
		memcpy(mem, value, file->subitem_size);
	}
	if(file->count>d) {
		memmove(&file->contents[d+1], &file->contents[d], sizeof(pair)*(file->count-d));
	}
	file->contents[d].key=c;
	file->contents[d].value=(sConfigFile*)mem;
	file->count++;
	return 0;
}
func _4d9d88() { func3(assoc_insert); }
static int _stdcall assoc_delete(sConfigFile* file, const char* key) {
	if(file->magic!=0xFEBAFEBA) return -1;
	int d;
	if(assoc_find(file, key, &d)==-1) return -1;
	free(file->contents[d].key);
	if(file->contents[d].value) free(file->contents[d].value);
	file->count--;
	if(file->count>d) memmove(&file->contents[d], &file->contents[d+1], sizeof(pair)*(file->count-d));
	return 0;
}
func _4d9ee8() { func2(assoc_delete); }
static int _stdcall assoc_read_long(FILE* file, uint32* read) {
	uint8 b[4];
	for(int i=0;i<4;i++) {
		b[i]=fgetc(file);
		if(b[i]==-1) return -1;
	}
	*read=(b[0]<<24) + (b[1]<<16) + (b[2]<<8) + b[3];
	return 0;
}
func _4da090() { func2(assoc_read_long); }
static int _stdcall assoc_read_assoc_array(FILE* file, sConfigFile* config) {
	uint32 d;
	if(assoc_read_long(file, &d)) return -1;
	config->count=d;
	if(assoc_read_long(file, &d)) return -1;
	config->max_count=d;
	if(assoc_read_long(file, &d)) return -1;
	config->subitem_size=d;
	if(assoc_read_long(file, &d)) return -1;
	config->contents=(pair*)d;
	return 0;
}
func _4da0f4() { func2(assoc_read_assoc_array); }
static void _stdcall assoc_register_mem() {}
func _4da498() { func0(assoc_register_mem); }

//func _4d9f84() { func0(assoc_copy); }
//func _4d9fe4() { func0(assoc_find_first); }
//func _4da158() { func0(assoc_load); }
//func _4da2ec() { func0(assoc_write_long); }
//func _4da360() { func0(assoc_write_assoc_array); }
//func _4da3a4() { func0(assoc_save); }
#endif