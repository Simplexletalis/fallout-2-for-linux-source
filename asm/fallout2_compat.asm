
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX Global functions which are called from the c code XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

;The program entry point
global entryPoint
extern FakeDSoundCreate
;extern _FakeDInputCreate@16
;extern _FakeDDrawCreate@12
entryPoint:
  mov dword [_focus], 1 ;We have focus
  mov dword [__51e430], FakeDSoundCreate ;supply the pointer to create a fake directsound interface
  ;mov dword [__51e42c], _FakeDInputCreate@16 ;supply the pointer to create the tmp direct input interface
  ;mov dword [__51e428], _FakeDDrawCreate@12
  mov dword [__51ede0], 1 ;So that movie tests to check that the ddraw device isn't null don't bomb out
  mov eax, [esp+12]
  mov [_window], eax ;The window pointer
  mov eax, [esp+4]
  mov edx, [esp+8]
  jmp _gnw_main;
align 4

;Called to perform a clean quit
global WantToQuit
WantToQuit:
  mov dword [__5186cc], 1
  retn
align 4

; XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX Overwritten functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXX

;debug_printf
extern DebugPrintf
__4c6f48:;		debug_printf_
  push edx
  push ecx
  lea eax, [esp+16]
  mov edx, [esp+12]
  push eax
  push edx
  call DebugPrintf
  add esp, 8
  pop ecx
  pop edx
  retn
align 4

; XXXXXXXXXXXXXXXXXXXXX
; XXX Input related XXX
; XXXXXXXXXXXXXXXXXXXXX


;int __fastcall dxinput_init()
__4e0400:;		dxinput_init_
;void __fastcall dxinput_exit()
__4e0478:;		dxinput_exit_
;int __fastcall dxinput_acquire_mouse()
__4e04e8:;		dxinput_acquire_mouse_
;int __fastcall dxinput_unacquire_mouse()
__4e0514:;		dxinput_unacquire_mouse_
;int __fastcall dxinput_acquire_keyboard()
__4e05a8:;		dxinput_acquire_keyboard_
;int __fastcall dxinput_unacquire_keyboard()
__4e05d4:;		dxinput_unacquire_keyboard_
  mov eax, 1
  retn
align 4

;int __fastcall dxinput_get_mouse_state(INTERNALMOUSESTATE *mouseState)
extern MouseState
__4e053c:;		dxinput_get_mouse_state_
  push edx
  push ecx
  push eax
  call MouseState
  add esp, 4
  pop ecx
  pop edx
  retn
align 4

;int __fastcall dxinput_flush_keyboard_buffer()
extern EmptyKeyQueue
__4e05fc:;		dxinput_flush_keyboard_buffer_
  push edx
  push ecx
  call EmptyKeyQueue
  mov eax, 1
  pop ecx
  pop edx
  retn
align 4

;int __fastcall dxinput_read_keyboard_buffer(int *key)
extern GetNextKey
__4e0650: push ebx;		dxinput_read_keyboard_buffer_
          push ecx
          push edx
          push esi
          push edi
          push ebp
          sub esp, byte 4
          mov ebx, eax
          test eax, eax
          jz __4e06fe
          mov ecx, dword [__51e468]
          cmp ecx, dword [__51e464]
          jg __4e06c7
          push byte 0
          lea ecx, [esp + 4]
          push ecx
          mov esi, dword 0x20
          push dword __6b2560
          mov dword [esp + 12], esi
          push byte 16
          call GetNextKey
		  add esp, 16
          test eax, eax
          jz __4e06b7
          cmp eax, byte 1
          jnz __4e06c7
__4e06b7: mov eax, dword [esp]
          xor edi, edi
          mov dword [__51e468], eax
          mov dword [__51e464], edi
__4e06c7: mov ebp, dword [__51e468]
          mov eax, dword [__51e464]
          cmp eax, ebp
          jge __4e06fe
          shl eax, byte 4
          add eax, dword __6b2560
          mov dl, byte  [eax]
          mov byte  [ebx], dl
          inc dword [__51e464]
          test byte  [eax + 4], byte -128
          setnz al
          and eax, dword 0xff
          mov byte  [ebx + 1], al
          mov eax, dword 0x1
          jmp __4e0700
__4e06fe: xor eax, eax
__4e0700: add esp, byte 4
          pop ebp
          pop edi
          pop esi
          pop edx
          pop ecx
          pop ebx
          retn
align 4

;Don't need these: only called by dxinput_init
;int __fastcall dxinput_mouse_init()
__4e070c:;		dxinput_mouse_init_
;void __fastcall dxinput_mouse_exit()
__4e078c:;		dxinput_mouse_exit_
;int __fastcall dxinput_keyboard_init()
__4e07b8:;		dxinput_keyboard_init_
;void __fastcall dxinput_keyboard_exit()
__4e0874:;		dxinput_keyboard_exit_
  int 3
align 4

;Silly thing hooks the keyboard and stops sdl from working
__4c9c28:;		GNW95_hook_keyboard_
  xor eax, eax
  retn
align 4

; XXXXXXXXXXXXXXXXXXXXX
; XXX Video related XXX
; XXXXXXXXXXXXXXXXXXXXX

;int __fastcall GNW95_init_mode_ex(int gWidth, int gHeight, int gDepth)
;Creates the window, sets the screen res and creates the primary surface. We do all that at startup now, so don't bother
extern showRect
extern clearScreen
__4cae1c:;		GNW95_init_mode_ex_
  xor eax, eax
  mov dword [__6ac9f0 + 0], eax
  mov dword [__6ac9f0 + 4], eax
  mov dword [__6ac9f0 + 8], dword 639
  mov dword [__6ac9f0 + 12], dword 479
  mov dword [__6ac7d8], eax
  mov dword [__6aca18], showRect
  mov dword [__6aca1c], ClearScreenWrapper
  mov dword [__6ac7dc], showRect
  retn
align 4

ClearScreenWrapper:
  push edx
  push ecx
  call clearScreen
  pop ecx
  pop edx
  retn
align 4

;int __fastcall GNW95_SetPaletteEntries(void *entries, int min, int max)
extern setPaletteEntries
__4cb310:;		GNW95_SetPaletteEntries_
  push ecx
  push ebx
  push edx
  push eax
  call setPaletteEntries
  add esp, 12
  pop ecx
  retn

;int __fastcall GNW95_SetPalette(byte *entries)
__4cb568:;		GNW95_SetPalette_
  push edx
  push ecx
  push dword 256
  push dword 0
  push eax
  call setPaletteEntries
  add esp, 12
  pop ecx
  pop edx
  retn
align 4

; XXXXXXXXXXXXXXXXXXXXX
; XXX Movie related XXX
; XXXXXXXXXXXXXXXXXXXXX

  ;movie flags:
  ;1 - don't fade the palette
  ;2 - if result is one, call _loadColorTable
  ;4 - call gsound_background_stop
  ;8 - call sub_450B50 at the start and 450B64 at the end

;That weird thing that does the virtualprotect
__4f4dd0:
;The polymorphic function at 4F7349, and the associated switch table
global _6b4033, _6b4037, _6b401b, _6b4017, _6b401f, _6b4023
extern _4f7359
__4f7438:;		$$R0006
__4f7440:;		$$R000A
__4f7444:;		$$R000D
__4f7464:;		$$R0019
__4f7478:;		$$R0028
__4f7488:;		$$R0034
__4f74a4:;		$$R003D
__4f754c:;		$$R002A
  retn
align 4

;The polymorphic function at 4f9f28 (unused)
__4f9f28:;		nfHPkDecomp_
__4fa014:;		$$R001A
__4fa01c:;		$$R001D
__4fa020:;		$$R0022
__4fa048:;		$$R003A
__4fa060:;		$$R0040
__4fa074:;		$$R004F
__4fa090:;		$$R005D
__4fa178:;		$$R006B
  int 3
  retn
align 4

extern FreeSurface
extern CreateSurface

;CreateMovieSurfaces [NONE]
global __4f5cb0;		nfConfig_
__4f5cb0: sub esp, byte 108;		nfConfig_
          cmp dword [__51ee00], byte 0
          push ebx
          push esi
          push edi
          jz __4f5cd4
          mov eax, dword [__51ee00]
          push eax
          call FreeSurface
		  add esp, 4
          mov dword [__51ee00], dword 0x0
global __4f5cd4
__4f5cd4: cmp dword [__51ee04], byte 0
          jz __4f5cf2
          mov eax, dword [__51ee04]
          push eax
          call FreeSurface
		  add esp, 4
          mov dword [__51ee04], dword 0x0
global __4f5cf2
__4f5cf2: mov eax, dword [esp + 124]
          mov ecx, dword [esp + 0x80]
          mov ebx, dword [esp + 0x84]
          mov byte  [__6b400d], al
          shl eax, byte 3
          mov byte  [__6b400c], cl
          mov byte  [__6b4016], bl
          mov dword [__6b3cfc], eax
          mov eax, ebx
          imul eax, ecx
          shl eax, byte 3
          cmp dword [__51ebd8], byte 0
          mov dword [__6b402f], eax
          jz __4f5d3a
          shr dword [__6b402f], byte 1
global __4f5d3a
__4f5d3a: lea edi, [esp + 12]
          xor eax, eax
          mov ecx, dword 0x1b
          rep stosd
          mov eax, dword [__6b3cfc]
          mov ecx, dword [__6b402f]
          mov dword [esp + 24], eax
          mov dword [esp + 20], ecx
          mov dword [esp + 12], dword 0x6c
          mov dword [esp + 16], dword 0x1007
          mov dword [esp + 116], dword 0x840
          mov dword [esp + 84], dword 0x20
          mov esi, dword [esp + 0x88]
          test esi, esi
          jz __4f5daf
          mov dword [esp + 88], dword 0x40
          mov dword [esp + 96], dword 0x10
          mov dword [esp + 100], dword 0x7c00
          mov dword [esp + 104], dword 0x3e0
          mov dword [esp + 108], dword 0x1f
          jmp __4f5dbf
global __4f5daf
__4f5daf: mov dword [esp + 88], dword 0x60
          mov dword [esp + 96], dword 0x8
global __4f5dbf
__4f5dbf: lea eax, [esp + 12]
          push dword __51ee00
          mov ecx, dword [__51ede0]
          push eax
          call CreateSurface
          add esp, 8
          test eax, eax
          jz __4f5de4
          xor eax, eax
          pop edi
          pop esi
          pop ebx
          add esp, byte 108
          retn
global __4f5de4
__4f5de4: lea eax, [esp + 12]
          push dword __51ee04
          mov ecx, dword [__51ede0]
          push eax
          call CreateSurface
          add esp, 8
          test eax, eax
          jz __4f5e09
          xor eax, eax
          pop edi
          pop esi
          pop ebx
          add esp, byte 108
          retn
global __4f5e09
__4f5e09: mov eax, dword [__6b3cfc]
          mov dword [__6b4027], esi
          imul eax, ebx
          sub eax, byte 8
          test esi, esi
          mov dword [__6b402b], eax
          jz __4f5e31
          shl dword [__6b3cfc], byte 1
          shl dword [__6b402b], byte 1
global __4f5e31
__4f5e31: mov ecx, dword [__6b3cfc]
          imul ecx, ebx
          lea eax, [ecx*8 + 0x0]
          mov dword [__6b3d00], eax
          sub eax, ecx
          mov dword [__6b3cec], eax
          call __4f731d;		nfPkConfig_
          mov eax, dword 0x1
          pop edi
          pop esi
          pop ebx
          add esp, byte 108
          retn
align 4

;LockMovieSurface
extern LockSurfaces
__4f5e60:
  push edx
  push ecx
  push _6b4037
  push _6b4033
  push dword [__51ee04]
  push dword [__51ee00]
  call LockSurfaces
  add esp, 16
  pop ecx
  pop edx
  retn
align 4

;UnlockMovieSurfaces
extern UnlockSurfaces
__4f5ef0:
  push edx
  push ecx
  push dword [__51ee04]
  push dword [__51ee00]
  call UnlockSurfaces
  add esp, 8
  pop ecx
  pop edx
  retn
align 4

;ReleaseMovieSurfaces
__4f6390:;		nfRelease_
  push edx
  push ecx
  push dword [__51ee04]
  call FreeSurface
  push dword [__51ee00]
  call FreeSurface
  add esp, 8
  mov dword [__51ee00], 0
  mov dword [__51ee04], 0
  pop ecx
  pop edx
  retn
align 4

;_movie_MVE_ShowFrame [NONE]
extern bltToScreen
global __486654;		movie_MVE_ShowFrame_
__486654: push ebx;		movie_MVE_ShowFrame_
          push esi
          push edi
          push ebp
          sub esp, dword 0x94
          mov esi, dword [esp + 0xa8]
          mov edi, dword [esp + 0xac]
          mov ebp, dword [esp + 0xb0]
          mov ebx, dword 0x6c
          mov eax, esp
          xor edx, edx
          call __4f0080;		memset_
          mov eax, dword [esp + 0xb4]
          mov edx, dword 0x6c
          mov dword [esp + 108], eax
          mov eax, dword [esp + 0xb8]
          mov ecx, dword [__638e14]
          mov dword [esp + 112], eax
          mov eax, dword [esp + 0xb4]
          mov ebx, dword [__638eb4]
          add eax, edi
          mov dword [esp], edx
          mov dword [esp + 116], eax
          mov eax, dword [esp + 0xb8]
          add ecx, ebx
          add eax, ebp
          mov edx, dword [__638eb0]
          mov dword [esp + 120], eax
          mov eax, dword [__638e10]
          mov ebx, dword [__638e14]
          add edx, eax
          mov eax, dword [__638e1c]
          sub eax, ebx
          mov ebx, eax
          mov eax, dword [__638e18]
          sub eax, dword [__638e10]
          mov dword [esp + 0x8c], eax
          lea eax, [ebx + 1]
          mov ebx, dword [esp + 0x8c]
          inc ebx
          mov dword [esp + 0x90], ebx
          mov ebx, dword [esp + 0xc0]
          sub eax, ebx
          mov ebx, eax
          mov eax, dword [__638e5c]
          shr ebx, 1
          test eax, eax
          jnz __486772
          test byte  [__638e78], byte 8
          jz __486743
          mov eax, dword [esp + 0x90]
          sub eax, dword [esp + 0xbc]
          shr eax, 1
          mov dword [esp + 0x80], ebx
          mov dword [esp + 124], eax
          jmp __48674e
global __486743
__486743: mov dword [esp + 0x80], ecx
          mov dword [esp + 124], edx
global __48674e
__48674e: mov eax, dword [esp + 124]
          add eax, dword [esp + 0xbc]
          mov ebx, dword [esp + 0xc0]
          mov dword [esp + 0x84], eax
          mov eax, dword [esp + 0x80]
          add eax, ebx
          jmp __4867dd
global __486772
__486772: test byte  [__638e78], byte 8
          jz __4867a5
          mov ecx, dword 0x3
          lea eax, [edi*4 + 0x0]
          xor edx, edx
          div ecx
          mov edx, dword [esp + 0x90]
          sub edx, eax
          mov eax, edx
          shr eax, 1
          mov dword [esp + 0x80], ebx
          mov dword [esp + 124], eax
          jmp __4867b0
global __4867a5
__4867a5: mov dword [esp + 0x80], ecx
          mov dword [esp + 124], edx
global __4867b0
__4867b0: mov ebx, dword 0x3
          lea eax, [edi*4 + 0x0]
          xor edx, edx
          div ebx
          mov edx, dword [esp + 124]
          add edx, eax
          mov eax, dword [esp + 0x80]
          mov dword [esp + 0x84], edx
          mov edx, dword [esp + 0xc0]
          add eax, edx
global __4867dd
__4867dd: mov dword [esp + 0x88], eax
          mov eax, dword [esp + 0xb4]
          mov dword [__638e54], eax
          mov eax, dword [esp + 0xb8]
          mov dword [__638e58], eax
          mov eax, dword [esp + 124]
          mov dword [__638e6c], eax
          mov eax, dword [esp + 0x80]
          mov ecx, dword [esp + 124]
          mov dword [__638e70], eax
          mov eax, dword [esp + 0x84]
          mov edx, dword [esp + 0x80]
          sub eax, ecx
          mov dword [__638e4c], ebp
          mov dword [__638e68], eax
          mov eax, dword [esp + 0x88]
          mov dword [__638e9c], esi
          sub eax, edx
          mov dword [__638e50], edi
          mov dword [__638e64], eax
global __4868c0
__4868c0: lea edx, [esp + 116 - 8]
          push edx
          push esi
          lea edx, [esp + 0x8c - 8]
          push edx
          call bltToScreen
          xor eax, eax
          add esp, 12
global __4868f3
__4868f3: add esp, dword 0x94
          pop ebp
          pop edi
          pop esi
          pop ebx
          retn
align 4

;_cleanupMovie [NONE]
extern LockSingleSurface, UnlockSingleSurface
global __486e98;		cleanupMovie_
__486e98: push ebx;		cleanupMovie_
          push ecx
          push edx
          push esi
          push edi
          push ebp
          sub esp, byte 116
          mov esi, eax
          cmp dword [__638ea4], byte 0
          jz __487111
global __486edd
__486edd: lea eax, [esp + 108]
          push eax
          lea eax, [esp + 116]
          push eax
          call __4f4bd0;		MVE_rmFrameCounts_
          add esp, byte 8
          mov edi, dword [esp + 108]
          push edi
          mov ebp, dword [esp + 116]
          push ebp
          push dword __50bb1c
          call __4c6f48;		debug_printf_
          mov eax, dword [__638e90]
          add esp, byte 12
          test eax, eax
          jz __486f26
          mov ebx, dword 0x313
          mov edx, dword __50bb0c
          call __484688;		myfree_
          xor edx, edx
          mov dword [__638e90], edx
global __486f26
__486f26: mov ebx, dword [__638e9c]
          test ebx, ebx
          jz __486fd2
          push byte 0
          push byte 1
          lea edx, [esp + 8]
          mov ecx, dword 0x6c
          push edx
          mov dword [esp + 12], ecx
          push byte 0
          mov eax, ebx
          push eax
          call LockSingleSurface
		  add esp, 20
          mov eax, dword [__638e50]
          imul eax, dword [__638e4c]
          mov ebx, dword 0x322
          mov edx, dword __50bb0c
          call __484660;		mymalloc_
          mov ebp, dword [esp + 16]
          mov edx, dword [__638e54]
          imul edx, ebp
          mov edi, dword [__638e50]
          push edi
          push eax
          mov dword [__638e90], eax
          push ebp
          mov eax, dword [__638e4c]
          mov ebx, dword [esp + 48]
          push eax
          mov ecx, dword [__638e58]
          add edx, ebx
          push edi
          add edx, ecx
          push edx
          call __4d36d4;		buf_to_buf_
          add esp, byte 24
          mov ecx, dword [esp + 36]
          mov eax, dword [__638e9c]
          push ecx
          mov edx, dword [eax]
          push eax
          call UnlockSingleSurface
		  add esp, 8
global __486fca
__486fca: xor edi, edi
          mov dword [__638e9c], edi
global __486fd2
__486fd2: test esi, esi
          jz __486fdb
          call __4f6240;		MVE_rmEndMovie_
global __486fdb
__486fdb: call __4f6350;		MVE_ReleaseMem_
          mov eax, dword [__638ea8]
          call __4c5eb4;		db_fclose_
          cmp dword [__638eac], byte 0
          jz __487051
          mov eax, dword [__5195b8]
          call __4d7918;		win_width_
          push eax
          mov eax, dword [__5195b8]
          call __4d7918;		win_width_
          mov edx, dword [__638eb4]
          imul edx, eax
          mov eax, dword [__5195b8]
          call __4d78b0;		win_get_buf_
          add eax, edx
          add eax, dword [__638eb0]
          push eax
          mov ebx, dword [__638e94]
          push ebx
          mov ecx, dword [__638e84]
          push ecx
          push ebx
          mov edi, dword [__638eac]
          push edi
          call __4d36d4;		buf_to_buf_
          add esp, byte 24
          mov edx, dword __638e20
          mov eax, dword [__5195b8]
          call __4d6f80;		win_draw_rect_
global __487051
__487051: mov ebp, dword [__638ebc]
          test ebp, ebp
          jz __487069
          mov eax, ebp
          call __4c5eb4;		db_fclose_
          xor eax, eax
          mov dword [__638ebc], eax
global __487069
__487069: mov edx, dword [__638ec0]
          test edx, edx
          jz __48708c
          mov ebx, dword 0x348
          mov eax, edx
          mov edx, dword __50bb0c
          call __484688;		myfree_
          xor ebx, ebx
          mov dword [__638ec0], ebx
global __48708c
__48708c: mov ecx, dword [__638eac]
          test ecx, ecx
          jz __4870af
          mov ebx, dword 0x34d
          mov edx, dword __50bb0c
          mov eax, ecx
          xor esi, esi
          call __484688;		myfree_
          mov dword [__638eac], esi
global __4870af
__4870af: mov esi, dword __50bb0c
global __4870b4
__4870b4: mov edi, dword [__638e74]
          test edi, edi
          jz __4870e8
          mov ebx, dword 0x353
          mov edx, esi
          mov eax, dword [edi]
          mov ecx, dword [edi + 8]
          call __484688;		myfree_
          mov ebx, dword 0x354
          mov eax, dword [__638e74]
          mov edx, esi
          call __484688;		myfree_
          mov dword [__638e74], ecx
          jmp __4870b4
global __4870e8
__4870e8: mov ebp, dword 0xffffffff
          mov dword [__638ea4], edi
          mov dword [__638e80], edi
          mov dword [__638e5c], edi
          mov dword [__638e7c], edi
          mov dword [__638e78], edi
          mov dword [__5195b8], ebp
global __487111
__487111: add esp, byte 116
          pop ebp
          pop edi
          pop esi
          pop edx
          pop ecx
          pop ebx
          retn
align 4

; XXXXXXXXXXXXXXXXXXXXXX
; XXX Window related XXX
; XXXXXXXXXXXXXXXXXXXXXX

;int (const char* title)
extern SetWindowTitle
__4d80d8:;		win_set_minimized_title_
  push edx
  push ecx
  push eax
  mov [__51e3dc], eax
  call SetWindowTitle
  add esp, 4
  pop ecx
  pop edx
  retn
align 4

;_GNW95_process_message [NONE]
extern RunMessageQueue
global __4c9cf0;		GNW95_process_message_
__4c9cf0: push ebx;		GNW95_process_message_
          push ecx
          push edx
          push esi
          push edi
          sub esp, byte 32
          cmp dword [_focus], byte 0
          jz __4c9da7
          call __4cbe18;		kb_is_disabled_
          test eax, eax
          jnz __4c9da7
global __4c9d12
__4c9d12: lea eax, [esp + 28]
          call __4e0650;		dxinput_read_keyboard_buffer_
          cmp eax, byte 1
          jnz __4c9d2b
          lea eax, [esp + 28]
          call __4c9e14;		GNW95_process_key_
          jmp __4c9d12
global __4c9d2b
__4c9d2b: call dword [_6c0200]
          mov ecx, eax
          xor ebx, ebx
          xor edx, edx
global __4c9d38
__4c9d38: mov esi, dword [edx + __6abf50]
          cmp esi, byte -1
          jz __4c9d9b
          mov eax, esi
          cmp esi, ecx
          ja __4c9d51
          mov esi, ecx
          sub esi, eax
          mov eax, esi
          jmp __4c9d56
global __4c9d51
__4c9d51: mov eax, dword 0x7fffffff
global __4c9d56
__4c9d56: mov di, word  [edx + __6abf54]
          mov esi, eax
          test di, di
          jz __4c9d6b
          mov eax, dword [__51e23c]
          jmp __4c9d70
global __4c9d6b
__4c9d6b: mov eax, dword [__51e240]
global __4c9d70
__4c9d70: cmp esi, eax
          jbe __4c9d9b
          mov byte  [esp + 29], byte 1
          lea eax, [esp + 28]
          mov byte  [esp + 28], bl
          call __4c9e14;		GNW95_process_key_
          mov ax, word  [edx + __6abf54]
          mov dword [edx + __6abf50], ecx
          inc eax
          mov word  [edx + __6abf54], ax
global __4c9d9b
__4c9d9b: inc ebx
          add edx, byte 8
          cmp ebx, dword 0x100
          jl __4c9d38
global __4c9da7
__4c9da7: call RunMessageQueue
          add esp, byte 32
          pop edi
          pop esi
          pop edx
          pop ecx
          pop ebx
          retn
align 4

; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX Any functions that we want to be nop's XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

global _removed_void_func
_removed_void_func
;This is a weird apparent nop in fallout - calls another function, which pops and then returns
__4e6aa5:;		getpow10
;The function that does the ereg gunk
__440dd0:;		annoy_user_
;Win32Shutdown
__413a00:;		autorun_mutex_destroy_
;GNW95_reset_mode
__4cb1b0:;		GNW95_reset_mode_
;SetMovieDevice
__4f4b80:
  retn
align 4

global _removed_unused_func
_removed_unused_func
;__CBeginThread_
__4efee3:;		__CBeginThread_
;__CEndThread_
__4effb7:;		__CEndThread_
;__CInitThread_
__4efcf7:;		__CInitThread_
;__FiniSema4s_
__4ec833:;		__FiniSema4s_
;__full_io_exit_
__4ef136:;		__full_io_exit_
;__setenvp_
__4ec312:;		__setenvp_
;__verify_pentium_fdiv_bug_
__4eb450:;		__verify_pentium_fdiv_bug_
;__checkIsDBCS_
__4ec102:;		__mbInitOnStartup_
;__InitFiles_
__4ef0bf:;		__InitFiles_
;__chk8087_
__4eb09b:;		__chk8087_
;__setEFGfmt_
__4f2168:;		__setEFGfmt_
;__clock_init_
__4f2198:;		__clock_init
;Jump table for 4E4040
__4e4094:
__4e415c:
__4e409e:
__4e40a8:
__4e4163:
__4e416a:
__4e417e:
;Some other sound pointer thingy
__4ac768:;		soundOpenData_
;j_close_
__4ac77c:;		soundCloseData_
;j_read_
__4ac760:;		soundReadData_
;j_write_
__4ac758:;		soundWriteData_
;j_lseek_
__4ac774:;		soundSeekData_
;j_tell_
__4ac750:;		soundTellData_
;soundFileSize_
__4ac71c:;		soundFileSize_
  int 3
  retn
align 4

global _removed_int_func
_removed_int_func
;Win32Startup - just creates another mutex
__4139c0:;		autorun_mutex_create_
  mov eax, 1
  retn
align 4

; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
; XXX Externified lib functions XXX
; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX





;signal functions


;assoc functions

%if 0
extern __4d9ba8 ;assoc_init;		assoc_init_
extern __4d9c0c ;assoc_resize;		assoc_resize_
extern __4d9c48 ;assoc_free;		assoc_free_
extern __4d9cc4 ;assoc_find;		assoc_find_
extern __4d9d5c ;assoc_search;		assoc_search_
extern __4d9d88 ;assoc_insert;		assoc_insert_
extern __4d9ee8 ;assoc_delete;		assoc_delete_
;extern __4d9f84 ;assoc_copy;		assoc_copy_
;extern __4d9fe4 ;assoc_find_first;		assoc_find_first_
extern __4da090 ;assoc_read_long;		assoc_read_long_
extern __4da0f4 ;assoc_read_assoc_array;		assoc_read_assoc_array_
;extern __4da158 ;assoc_load;		assoc_load_
;extern __4da2ec ;assoc_write_long;		assoc_write_long_
;extern __4da360 ;assoc_write_assoc_array;		assoc_write_assoc_array_
;extern __4da3a4 ;assoc_save;		assoc_save_
extern __4da498 ;assoc_register_mem;		assoc_register_mem_
%endif


%include "exports.asm"
