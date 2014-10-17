
#define uint8  unsigned char
#define uint16 unsigned short
#define uint32 unsigned int

#define crash *(int*)0=0
#define _stdcall __attribute__((stdcall))
#define _cdecl

#ifdef _WIN32
#define CCONV1(a) __##a
#define CCONV2(a)
#else
#define CCONV1(a)
#define CCONV2(a) __attribute__((a))
#endif
