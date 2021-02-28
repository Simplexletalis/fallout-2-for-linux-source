#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include "files.h"
#include "zlib/zlib.h"

char work_dir[PATH_MAX];

int updateWorkingDir() {
  getcwd(work_dir, sizeof(work_dir));
  printf("Current working dir: %s\n", work_dir);
}

const char* getWorkingDir() {
  return work_dir;
}

long file_length(char *f)
{
    struct stat st;
    if (stat(f, &st)) {
      return -1;
    }
    return st.st_size;
}

long file_length_stream(FILE *stream)
{
    int fd = fileno(stream);
    if(fd == -1) {
      return -1;
    }
    struct stat st;
    if (fstat(fd, &st)) {
      return -1;
    }
    return st.st_size;
}


void splitpath (
   const char *path,  // Path Input
   char *drv,       // Drive     : Output
   char *dir,         // Directory : Output
   char *name,       // Filename  : Output
   char *ext          // Extension : Output
)
{
    if(drv)
	*drv = '\0';

    const char* end;
    const char* p;
    const char* s;

    end = path + strlen(path);

    for(p=end; p>path && *--p!='\\' && *p!='/'; )
	if (*p == '.') {
	    end = p;
	    break;
	}

    if (ext)
	for(s=end; (*ext=*s++); )
	    ext++;

    for(p=end; p>path; )
	if (*--p=='\\' || *p=='/') {
	    p++;
	    break;
	}

    if (name) {
	for(s=p; s<end; )
	    *name++ = *s++;

	*name = '\0';
    }

    if (dir) {
	for(s=path; s<p; )
	    *dir++ = *s++;

	*dir = '\0';
    }
}

void makepath(
   char *path,
   const char *drive,
   const char *dir,
   const char *fname,
   const char *ext
) {

  printf("drive: %s, dir, %s, fname: %s, ext: %s\n", drive, dir, fname, ext);
    char *p = path;
 
     if ( !path )
         return;
 
     if (drive && drive[0])
     {
         *p++ = drive[0];
         *p++ = ':';
     }
     if (dir && dir[0])
     {
         size_t len = strlen(dir);
         memmove(p, dir, len);
         p += len;
         if (p[-1] != '/' && p[-1] != '\\')
             *p++ = '/';
     }
     if (fname && fname[0])
     {
         size_t len = strlen(fname);
         memmove(p, fname, len);
         p += len;
     }
     if (ext && ext[0])
     {
         if (ext[0] != '.')
             *p++ = '.';
         strcpy(p, ext);
     }
     else
         *p = '\0';
}



int startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? 0 : memcmp(pre, str, lenpre) == 0;
}

char* changePath(char* newname, const char* oldname) {
  strcpy(newname, oldname);
  char *p = newname;
  if(startsWith(work_dir, p) == 1) {
    p += strlen(work_dir);
  }
  while(*p) {
    *p = tolower(*p);
    if(*p == '\\') {
      *p = '/';
    }
    p++;
  }
  newname[strcspn(newname, "\r\n")] = 0;
  return newname;
}

int mkdir1(const char *pathname) {
  char newpath[PATH_MAX];
  changePath(newpath, pathname);
  printf("mkdir: %s, translated: %s\n", pathname, newpath);
  return mkdir(newpath, 0777);
}


int chdir1(const char *pathname) {
  char newpath[PATH_MAX];
  changePath(newpath, pathname);
  printf("chdir: %s, translated: %s\n", pathname, newpath);
  return chdir(newpath);
}

int rename1(const char *oldpath, const char *newpath) {
  char frompath[PATH_MAX];
  char topath[PATH_MAX];

  changePath(frompath, oldpath);
  changePath(topath, newpath);

  printf("rename: old [%s -> %s] new: [%s -> %s]\n", oldpath, frompath, newpath, topath);

  return rename(frompath, topath);
}

int remove1(const char *pathname) {
  char newpath[PATH_MAX];
  changePath(newpath, pathname);
  printf("remove: %s, translated: %s\n", pathname, newpath);
  return remove(newpath);
}

int access1(const char *pathname, int mode) {
  char newpath[PATH_MAX];
  changePath(newpath, pathname);
  printf("access: %s, translated: %s mode: %s\n", pathname, newpath, mode);
  return access(newpath, mode);
}

FILE *fopen1(const char *path, const char *mode) {
  char newpath[PATH_MAX];
  changePath(newpath, path);
  FILE* fd = fopen(newpath, mode);
  //printf("Open: %s -> %s FD: %x mode: %s\n", path, newpath, fd, mode);
  return fd;
}

gzFile gzopen1(const char *path, const char *mode) {
  char newpath[PATH_MAX];
  strcpy(newpath, path);
  for(int i = 0; newpath[i]; i++){
    newpath[i] = tolower(newpath[i]);
    if(newpath[i] == '\\') {
      newpath[i] = '/';
    }
  }
  newpath[strcspn(newpath, "\r\n")] = 0;
  printf("gzOpen: %s -> %s mode: %s\n", path, newpath, mode);
  return gzopen(newpath, mode);
}

int gzprintf1(gzFile file, const char *format, va_list va)
{
  printf("gzprintf1\n");

  char buf[4096];
  int len;

  vsnprintf(buf, sizeof(buf), format, va);

  len = strlen(buf); /* some *sprintf don't return the nb of bytes written */
  if (len <= 0) return 0;

  return gzwrite(file, buf, (unsigned)len);
}
