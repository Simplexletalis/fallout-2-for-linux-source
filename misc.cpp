#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "interop_macros.h"

typedef void (CCONV1(cdecl) *atexit_f)(void) CCONV2(cdecl);

extern "C" {
	int CCONV1(cdecl) h_atexit(atexit_f f) CCONV2(cdecl);
}

int CCONV1(cdecl) h_atexit(atexit_f f) { atexit(f); return 0; }

extern "C" char *strlwr1(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

extern "C" char *strupr1(char *str) {
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = toupper((unsigned char)*p);
      p++;
  }

  return str;
}

extern "C" char * utoa1(unsigned int n, char * buffer, int radix) {
  //NOTE: radis not used
  sprintf(buffer, "%u", n);
  return buffer;
}

extern "C" char * itoa1(unsigned int n, char * buffer, int radix) {
  //NOTE: radis not used
  sprintf(buffer, "%d", n);
  return buffer;
}

int dinfo_compare(void const *lhs, void const *rhs)
{
   char* a = (char*)lhs;
   char* b = (char*)*((int*)rhs);

   //printf("compare %s, %s\n", a, b);
   return strcasecmp(a, b);
}

extern "C" void *dinfo_search(const void *key, const void *base, size_t nmemb, size_t size, void*) {
  //printf("key: %s, base: %p num: %d, size: %d\n", key, base, nmemb, size);
  char newkey[255];
  strcpy(newkey, (char*)key);
  for(int i = 0; newkey[i]; i++){
    if(newkey[i] == '/') {
      newkey[i] = '\\';
    }
  }
  newkey[strcspn(newkey, "\r\n")] = 0;
  //printf("dinfo_search: %s, -> %s\n", (char*)key, newkey);
  //for(int i =0; i< nmemb; i++) {
    //printf("%d --- %s\n", nmemb - i,(char*)*((int*)(base+i*size)));
  //}
  return bsearch(&newkey, base, nmemb, size, dinfo_compare);
}


extern "C" void *bsearch1(const void *key, const void *base, size_t nmemb, size_t size, void*) {
  printf("key: %s, base: %p num: %d, size: %d\n", key, base, nmemb, size);
  //printf("dinfo_search: %s, translated: %s\n", (char*)key, newkey);
  for(int i =0; i< nmemb; i++) {
    printf("%d --- %s\n", nmemb - i,(char*)*((int*)(base+i*size)));
  }
  return 0; //bsearch(key, base, nmemb, size, dinfo_compare);
}

extern "C" void qsort1(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
  printf("num: %d, size: %d", nmemb, size);
}


