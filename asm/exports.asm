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
extern _ff_d1, _ff_d2, _h_pow@0, _h_atan2@0
__4deb3a:
  pushad;
  fstp qword [_ff_d1];
  fstp qword [_ff_d2];
  call _h_pow@0;
  popad;
  retn;
align 4

__4decb5:
  pushad;
  fstp qword [_ff_d1];
  fstp qword [_ff_d2];
  call _h_atan2@0;
  popad;
  retn;
align 4

; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX memory functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern _malloc, _realloc, _free, _memset, _memmove
__4c5ad0:
__4c5ad8:
__4ef1c5: func1 _malloc
__4c5b50:
__4c5b58:
__4f1669: func2 _realloc
__4c5c24:
__4c5c2c:
__4ef2b4: func1 _free
__4effe1: func3 _memmove
__4f0080: func3 _memset

; XXXXXXXXXXXXXXXXXXXXXX
; XXX File functions XXX
; XXXXXXXXXXXXXXXXXXXXXX
extern _fclose, _fflush, _fopen, _h_fprintf, _fgetc, _fgets, _fputc, _fputs, _ungetc, _fread, _fwrite, _fgetpos, _fseek, _fsetpos, _ftell
extern _rewind, _filelength, _rename, _remove, _access, _fdopen
__4eb810: func1 _fclose
__4e03e6: func1 _fflush
__4ead56: func2 _fopen
__4ebd4d: func3 _h_fprintf
__4f11d5: func1 _fgetc
__4f0cdb: func3 _fgets
__4f134e: func2 _fputc
__4e1037: func2 _fputs
__4e61e2: func2 _ungetc
__4f018e: func4 _fread
__4f0fa9: func4 _fwrite
__4e627d: func2 _fgetpos
__4f0428: func3 _fseek
__4e6290: func2 _fsetpos
__4e3afe: func1 _ftell
__4f1411: func1 _rewind
__4f038d: func1 _filelength
__4f0521: func2 _rename
__4f1daf: func1 _remove
__4f0f16: func2 _access
__4e39fc: func2 _fdopen

; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX random number functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

extern _rand, _srand
__4f1740: func0 _rand
__4f1764: func1 _srand
;extern __4f1736 ;initrandnext

; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX Signal functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern _h_fatal_runtime_error, _h_GNWSystemError, _h_get_errno_ptr, _h_longjmp, _h_assert
__4e3f10: xor eax, eax
          retn
		  align 4
__4ec2f5: func2 _h_fatal_runtime_error
__4d8200: func1 _h_GNWSystemError
__4f11aa: func0 _h_get_errno_ptr

extern _setjmp_ptr
__4f150e:
		cmp dword [eax], 0x1bcd1234
		je .jump
		mov dword [eax], 0x1bcd1234
		pushad
		mov edi, eax
		push 64
		call _malloc
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
		call dword [_setjmp_ptr]
		add esp, 4
		push dword [edi+8]
		mov ecx, [edi+12]
		mov edx, [edi+16]
		mov edi, [edi+20]
		retn
		align 4
__4f154d: func2 _h_longjmp
__4e02ef: func4 _h_assert

; XXXXXXXXXXXXXXXXXXXXXX
; XXX Time functions XXX
; XXXXXXXXXXXXXXXXXXXXX
extern _localtime, _time, _clock
__4f1652: func1 _localtime
__4f15d2: func1 _time
__4f21a3: func0 _clock
;extern __4f2198 ;clock_init

; XXXXXXXXXXXXXXXXXXXXXX
; XXX Misc functions XXX
; XXXXXXXXXXXXXXXXXXXXXX
extern _h_atexit, _exit
__4e0349: func1 _h_atexit
__4e660f: func1 _exit

; XXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX directory functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXX

extern _opendir, _readdir, _closedir, _chdir, _mkdir, _getcwd, __splitpath
__4ebfa9: func1 _opendir
__4ebfb6: func1 _readdir
__4ec029: func1 _closedir
__4e635a: func1 _chdir
__4f118f: func1 _mkdir
__4e62e1: func2 _getcwd
__4f0a25: func5 __splitpath
;extern __4e65dd ;rmdir

; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX string functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern _strncat, _strstr, _strdup, _strtok, _stricmp, _atoi, _strpbrk, _strncpy, _strrchr, _strtod, _vsprintf
extern _sscanf, _vsprintf, _vprintf
__4f0f6a: func3 _strncat
__4f1450: func2 _strstr
__4c5a80: func1 _strdup
__4e629c: func1 _strdup
__4f0dc6: func2 _strtok
__4dece6: func2 _stricmp
__4f0098: func1 _atoi
__4f0105: func2 _strpbrk
__4f014f: func3 _strncpy
__4f0174: func2 _strrchr
__4da4cd: func2 _strtod
;visual studio doesn't have a vsscanf, so we need to improvise a little. (But not as much as setjmp, thankfully...)
section .bss
sscanf_ret: resb 4
sscanf_ecx: resb 4
sscanf_edx: resb 4
section .text
__4f0ef5:
		pop eax;
		mov [sscanf_ret], eax;
		mov [sscanf_ecx], ecx;
		mov [sscanf_edx], edx;
		call _sscanf;
		mov ecx, [sscanf_ecx];
		mov edx, [sscanf_edx];
		jmp dword [sscanf_ret];
		align 4
__4f143b: func3 _vsprintf
__4f0041:
		push edx
		push ecx
		lea eax, [esp+20]
		mov edx, [esp+16]
		mov ecx, [esp+12]
		push eax
		push edx
		push ecx
		call _vsprintf
		add esp, 12
		pop ecx
		pop edx
		retn
		align 4
__4f0f48:
		push edx
		push ecx
		lea eax, [esp+16]
		mov edx, [esp+12]
		push eax
		push edx
		call _vprintf
		add esp, 8
		pop ecx
		pop edx
		retn
		align 4
; __4f0ece: func vsscanf