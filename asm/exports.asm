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
__4deb3a:
  pushad;
  fstp qword [ff_d1];
  fstp qword [ff_d2];
  call h_pow;
  popad;
  retn;
align 4

__4decb5:
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
__4c5ad0:
__4c5ad8:
__4ef1c5: func1 malloc
__4c5b50:
__4c5b58:
__4f1669: func2 realloc
__4c5c24:
__4c5c2c:
__4ef2b4: func1 free
__4effe1: func3 memmove
__4f0080: func3 memset

; XXXXXXXXXXXXXXXXXXXXXX
; XXX File functions XXX
; XXXXXXXXXXXXXXXXXXXXXX
extern fclose, fflush, fopen, h_fprintf, fgetc, fgets, fputc, fputs, ungetc, fread, fwrite, fgetpos, fseek, fsetpos, ftell
extern rewind, file_length, rename, remove, access, fdopen
__4eb810: func1 fclose
__4e03e6: func1 fflush
_fopen:   func2 fopen
__4ebd4d: func3 h_fprintf
__4f11d5: func1 fgetc
__4f0cdb: func3 fgets
__4f134e: func2 fputc
__4e1037: func2 fputs
__4e61e2: func2 ungetc
__4f018e: func4 fread
__4f0fa9: func4 fwrite
__4e627d: func2 fgetpos
__4f0428: func3 fseek
__4e6290: func2 fsetpos
__4e3afe: func1 ftell
__4f1411: func1 rewind
__4f038d: func1 file_length
__4f0521: func2 rename
__4f1daf: func1 remove
__4f0f16: func2 access
__4e39fc: func2 fdopen

; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX random number functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

extern rand, srand
__4f1740: func0 rand
__4f1764: func1 srand
;extern __4f1736 ;initrandnext

; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX Signal functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern h_fatal_runtime_error, h_GNWSystemError, h_get_errno_ptr, h_longjmp, h_assert
__4e3f10: xor eax, eax
          retn
		  align 4
__4ec2f5: func2 h_fatal_runtime_error
_errorOut:func1 h_GNWSystemError
__4f11aa: func0 h_get_errno_ptr

extern setjmp_ptr
__4f150e:
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
__4f154d: func2 h_longjmp
__4e02ef: func4 h_assert

; XXXXXXXXXXXXXXXXXXXXXX
; XXX Time functions XXX
; XXXXXXXXXXXXXXXXXXXXX
extern localtime, time, clock
__4f1652: func1 localtime
__4f15d2: func1 time
__4f21a3: func0 clock
;extern __4f2198 ;clock_init

; XXXXXXXXXXXXXXXXXXXXXX
; XXX Misc functions XXX
; XXXXXXXXXXXXXXXXXXXXXX
extern h_atexit, exit
__4e0349: func1 h_atexit
__4e660f: func1 exit

; XXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX directory functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXX

extern opendir, readdir, closedir, chdir, mkdir, getcwd, splitpath
__4ebfa9: func1 opendir
__4ebfb6: func1 readdir
__4ec029: func1 closedir
__4e635a: func1 chdir
__4f118f: func1 mkdir
__4e62e1: func2 getcwd
__4f0a25: func5 splitpath
;extern __4e65dd ;rmdir

; XXXXXXXXXXXXXXXXXXXXXXXX
; XXX string functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXX

extern strncat, strstr, strdup, strtok, strcmp, atoi, strpbrk, strncpy, strrchr, strtod, vsprintf
extern sscanf, vsprintf, vprintf
__4f0f6a: func3 strncat
__4f1450: func2 strstr
__4c5a80: func1 strdup
__4e629c: func1 strdup
__4f0dc6: func2 strtok
__4dece6: func2 strcmp
__4f0098: func1 atoi
__4f0105: func2 strpbrk
__4f014f: func3 strncpy
__4f0174: func2 strrchr
__4da4cd: func2 strtod
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
		call sscanf;
		mov ecx, [sscanf_ecx];
		mov edx, [sscanf_edx];
		jmp dword [sscanf_ret];
		align 4
__4f143b: func3 vsprintf
__4f0041:
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
__4f0f48:
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
; __4f0ece: func vsscanf
