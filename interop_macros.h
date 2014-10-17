static int tmpresult;

#ifdef NO_HOOKS
#define MK_NAME(a) a
#else
#define MK_NAME(a) h_##a
#endif

#define func0(b) __asm { __asm pushad \
	__asm call MK_NAME(b) \
	__asm mov tmpresult, eax \
	__asm popad \
	__asm mov eax, tmpresult \
	__asm retn }
#define func1(b) __asm { __asm pushad \
	__asm push eax \
	__asm call MK_NAME(b) \
	__asm mov tmpresult, eax \
	__asm popad \
	__asm mov eax, tmpresult \
	__asm retn }
#define func2(b) __asm { __asm pushad \
	__asm push edx \
	__asm push eax \
	__asm call MK_NAME(b) \
	__asm mov tmpresult, eax \
	__asm popad \
	__asm mov eax, tmpresult \
	__asm retn }
#define func3(b) __asm { __asm pushad \
	__asm push ebx \
	__asm push edx \
	__asm push eax \
	__asm call MK_NAME(b) \
	__asm mov tmpresult, eax \
	__asm popad \
	__asm mov eax, tmpresult \
	__asm retn }
#define func4(b) __asm { \
	__asm push ecx \
	__asm push ebx \
	__asm push edx \
	__asm push eax \
	__asm call MK_NAME(b) \
	__asm retn }
#define func5(b) __asm { \
	__asm push edi \
	__asm mov edi, [esp+8] \
	__asm push edi \
	__asm push ecx \
	__asm push ebx \
	__asm push edx \
	__asm push eax \
	__asm call MK_NAME(b) \
	__asm pop edi \
	__asm retn 4 }

#ifdef __cplusplus
#define func extern "C" int __declspec(naked)
#else
#define func int __declspec(naked)
#endif