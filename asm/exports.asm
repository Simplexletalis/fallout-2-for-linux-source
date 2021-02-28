%macro func0 1
push edx
push ecx
call %1
pop ecx
pop edx
retn
align 4
%endmacro

%macro func1 1
push edx
push ecx
push eax
call %1
add esp, 4
pop ecx
pop edx
retn
align 4
%endmacro

%macro func2 1
push ecx
push edx
push eax
call %1
add esp, 8
pop ecx
retn
align 4
%endmacro

%macro func3 1
push ecx
push ebx
push edx
push eax
call %1
add esp, 12
pop ecx
retn
align 4
%endmacro

%macro func4 1
push ecx
push ebx
push edx
push eax
call %1
add esp, 16
retn
align 4
%endmacro

%macro func5 1
push edi
mov edi, [esp+8]
push edi
push ecx
push ebx
push edx
push eax
call %1
add esp, 20
pop edi
retn 4
align 4
%endmacro

; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX floating point functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
extern ff_d1, ff_d2, h_pow, h_atan2
__4deb3a:;		IF@DPOW
  pushad;
  fstp qword [ff_d1];
  fstp qword [ff_d2];
  call h_pow;
  popad;
  retn;
align 4

__4decb5:;		IF@ATAN2
  pushad;
  fstp qword [ff_d1];
  fstp qword [ff_d2];
  call h_atan2;
  popad;
  retn;
align 4

; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX memory functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern malloc, realloc, free, memset, memmove
__4c5ad0:;		mem_malloc_
__4c5ad8:;		my_malloc_
__4ef1c5: func1 malloc;		nmalloc_
__4c5b50:;		mem_realloc_
__4c5b58:;		my_realloc_
__4f1669: func2 realloc;		nrealloc_
__4c5c24:;		mem_free_
__4c5c2c:;		my_free_
__4ef2b4: func1 free;		nfree_
__4effe1: func3 memmove;		memmove_
__4f0080: func3 memset;		memset_

; XXXXXXXXXXXXXXXXXXXXXX
; XXX File functions XXX
; XXXXXXXXXXXXXXXXXXXXXX
extern fclose, fflush, fopen1, vfprintf, fgetc, fgets, fputc, fputs, ungetc, fread, fwrite, fgetpos, fseek, fsetpos, ftell
extern rewind, file_length, rename1, remove1, access1, fdopen, file_length_stream
__4eb810: func1 fclose;		fclose_
__4e03e6: func1 fflush;		fflush_
_fopen:   func2 fopen1
__4f11d5: func1 fgetc;		fgetc_
__4f0cdb: func3 fgets;		fgets_
__4f134e: func2 fputc;		fputc_
__4e1037: func2 fputs;		fputs_
__4e61e2: func2 ungetc;		ungetc_
__4f018e: func4 fread;		fread_
__4f0fa9: func4 fwrite;		fwrite_
__4e627d: func2 fgetpos;		fgetpos_
__4f0428: func3 fseek;		fseek_
__4e6290: func2 fsetpos;		fsetpos_
__4e3afe: func1 ftell;		ftell_
__4f1411: func1 rewind;		rewind_
__4f038d: func1 file_length;		filelength_
__4f0521: func2 rename1;		rename_
__4f1daf: func1 remove1;		unlink_
__4f0f16: func2 access1;		access_
__4e39fc: func2 fdopen;		fdopen_


__filelengthstream: func1 file_length_stream

extern utoa1, itoa1
__4f0c76: func3 utoa1;		utoa_
__4f0cc0: func3 itoa1;		itoa_


extern toupper, tolower
__4f0d7d: func1 toupper;		toupper_
__4f0e6a: func1 tolower;		tolower_


; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX fpattern                XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
extern fpattern_isvalid, fpattern_match
__4eb9e4: func1 fpattern_isvalid ;		fpattern_isvalid_
__4ebcf8: func2 fpattern_match;		fpattern_match_


; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX random number functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
extern rand, srand
__4f1740: func0 rand;		rand_
__4f1764: func1 srand;		srand_
;extern __4f1736 ;initrandnext;		initrandnext_

; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX Signal functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern h_fatal_runtime_error, h_GNWSystemError, h_get_errno_ptr, h_longjmp, h_assert
__4e3f10: xor eax, eax;		signal_
          retn
		  align 4
__4ec2f5: func2 h_fatal_runtime_error;		__fatal_runtime_error_
_errorOut:func1 h_GNWSystemError
__4f11aa: func0 h_get_errno_ptr;		__get_errno_

extern setjmp_ptr
__4f150e:;		_setjmp_
		cmp dword [eax], 0x1bcd1234
		je .jump
		mov dword [eax], 0x1bcd1234
		pushad
		mov edi, eax
		push 64
		call malloc
		add esp, 4
		mov [edi+4], eax
		popad
.jump:
		;Wheeeeee magic
		pop dword [eax+8]
		mov dword [eax+12], ecx
		mov dword [eax+16], edx
		mov dword [eax+20], edi
		mov edi, eax
		push dword [eax+4]
		call dword [setjmp_ptr]
		add esp, 4
		push dword [edi+8]
		mov ecx, [edi+12]
		mov edx, [edi+16]
		mov edi, [edi+20]
		retn
		align 4
__4f154d: func2 h_longjmp;		longjmp_
__4e02ef: func4 h_assert;		assert_

; XXXXXXXXXXXXXXXXXXXXXX
; XXX Time functions XXX
; XXXXXXXXXXXXXXXXXXXXX
extern localtime, time, clock
__4f1652: func1 localtime;		localtime_
__4f15d2: func1 time;		time_
__4f21a3: func0 clock;		get_clock_time_
;extern __4f2198 ;clock_init;		__clock_init

; XXXXXXXXXXXXXXXXXXXXXX
; XXX Misc functions XXX
; XXXXXXXXXXXXXXXXXXXXXX
extern h_atexit, exit
__4e0349: func1 h_atexit;		atexit_
__4e660f: func1 exit;		exit_

; XXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX directory functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXX

extern opendir1, readdir1, closedir1, chdir1, mkdir1, getcwd, splitpath, makepath
__4ebfa9: func1 opendir1;		opendir_
__4ebfb6: func1 readdir1;		readdir_
__4ec029: func1 closedir1;		closedir_
__4e635a: func1 chdir1;		chdir_
__4f118f: func1 mkdir1;		mkdir_
__4e62e1: func2 getcwd;		getcwd_
__4f0a25: func5 splitpath;		_splitpath_
__4e63f2: func5 makepath;		_makepath_

;extern __4e65dd ;rmdir;		rmdir_

; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX zlib                     XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
extern gzopen1, gzputc, gzgetc, gzgets, gzputs, gzread, gzwrite, gzseek, gztell, gzrewind, gzeof, gzclose
__4ddb88: func2 gzopen1;		gzopen_
__4de1f4: func2 gzputc;		gzputc_
__4de080: func1 gzgetc;		gzgetc_
__4de0a8: func3 gzgets;		gzgets_
__4de220: func2 gzputs;		gzputs_
__4dde84: func3 gzread;		gzread_
__4de100: func3 gzwrite;		gzwrite_
__4de334: func3 gzseek;		gzseek_
__4de4e4: func1 gztell;		gztell_
__4de480: func1 gzrewind;		gzrewind_
__4de4f8: func1 gzeof;		gzeof_
__4de574: func1 gzclose;		gzclose_

extern gzprintf1
gzvprintf1: func3 gzvprintf1;		gzprintf_

__4de18c:;		gzprintf_
		push edx
		push ecx
		lea eax, [esp+20]
		mov edx, [esp+16]
		mov ecx, [esp+12]
		push eax
		push edx
		push ecx
		call gzvprintf1
		add esp, 12
		pop ecx
		pop edx
		retn
		align 4


extern inflateInit_, inflate, inflateEnd
__4e3434: func3 inflateInit_;		inflateInit_
__4e3480: func2 inflate;		inflate_
__4e32cc: func1 inflateEnd;		inflateEnd_



; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX string functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern strncat, strstr, strdup, strtok, atoi, strpbrk, strncpy, strrchr, strtod, vsprintf
extern sscanf, vsprintf, vprintf, strlwr1, strupr1, strspn, strcspn, strcmp, strcasecmp, strncmp, strncasecmp
__4f0f6a: func3 strncat;		strncat_
__4f1450: func2 strstr;		strstr_
__4c5a80: func1 strdup;		mem_strdup_
__4e629c: func1 strdup;		strdup_
__4f0dc6: func2 strtok;		strtok_
__4dece6: func2 strcasecmp;		stricmp_
__4f0098: func1 atoi;		atoi_
__4f0105: func2 strpbrk;		strpbrk_
__4f014f: func3 strncpy;		strncpy_
__4f0174: func2 strrchr;		strrchr_
__4da4cd: func2 strtod;		strtod_
__4f0afe: func1 strlwr1;		strlwr_
__4f0da1: func1 strupr1;		strupr_
__4f0b23: func2 strspn;		strspn_
__4f0b67: func2 strcspn;		strcspn_
__4f0bb0: func2 strcmp;		strcmp_
__4f0e78: func3 strncmp;		strncmp_
__4f16e3: func3 strncasecmp;		strnicmp_


extern strerror
__4f11be: func1 strerror;		strerror_

;extern qsort1
;__4f05b6: func4 qsort1;		qsort_

;extern bsearch1
;__4f1774: func5 bsearch1;		bsearch_


extern dinfo_search
__4f1774_dinfo_search: func5 dinfo_search;		bsearch_



;visual studio doesn't have a vsscanf, so we need to improvise a little. (But not as much as setjmp, thankfully...)
section .bss
sscanf_ret: resb 4
sscanf_ecx: resb 4
sscanf_edx: resb 4
section .text
__4f0ef5:;		sscanf_
		pop eax;
		mov [sscanf_ret], eax;
		mov [sscanf_ecx], ecx;
		mov [sscanf_edx], edx;
		call sscanf;
		mov ecx, [sscanf_ecx];
		mov edx, [sscanf_edx];
		jmp dword [sscanf_ret];
		align 4
__4f143b: func3 vsprintf;		vsprintf_
__4f0041:;		sprintf_
		push edx
		push ecx
		lea eax, [esp+20]
		mov edx, [esp+16]
		mov ecx, [esp+12]
		push eax
		push edx
		push ecx
		call vsprintf
		add esp, 12
		pop ecx
		pop edx
		retn
		align 4
__4f0f48:;		printf_
		push edx
		push ecx
		lea eax, [esp+16]
		mov edx, [esp+12]
		push eax
		push edx
		call vprintf
		add esp, 8
		pop ecx
		pop edx
		retn
		align 4
; __4f0ece: func vsscanf;		vsscanf_

__4e61dd: func3 vfprintf;		vfprintf_
__4f0d56:;		fprintf_
		push edx
		push ecx
		lea eax, [esp+20]
		mov edx, [esp+16]
		mov ecx, [esp+12]
		push eax
		push edx
		push ecx
		call vfprintf
		add esp, 12
		pop ecx
		pop edx
		retn
		align 4
