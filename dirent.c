#include "files.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <stdio.h>
#include <linux/limits.h>

#include "dirent.h"

int globerr(const char *path, int eerrno)
{
    fprintf(stderr, "%s: %s\n", path, strerror(eerrno));
    return 0;	/* let glob() keep going */
}

DIR * opendir1(const char * dirname)
{
   char * name;
   int len;
   DIR * dir;
   int apiret;

   char newdirname[PATH_MAX];
   changePath(newdirname, dirname);

   printf("diropen: %s -> %s\n", dirname, newdirname);


   len=strlen(newdirname);
   if( (name=malloc(len+5))==NULL ) {
     errno=ENOMEM;
     return NULL;
   }
   strcpy(name, newdirname);
/*
   if( len-- && name[len]!=':' && name[len]!='\\' && name[len]!='/' )
     strcat(name,"\\*.*");
   else
     strcat(name,"*.*");
*/

   if( (dir=malloc(sizeof(DIR)))==NULL ) {
     errno=ENOMEM;
     free(name);
     return NULL;
   }

  int flags = 0;

  if(glob(name, flags, globerr, &(dir->glob_res)) !=0 ) {
    free(name);
    free(dir);
    return NULL;
  }

  dir->glob_curr_entry = 0;
  dir->dirname=name;

  return dir;
}


struct dirent * readdir1(DIR * dir)
{
  printf("readdir\n");
  static char *name_ptr = 0;
  int i = dir->glob_curr_entry;

  if (i >= dir->glob_res.gl_pathc)
    return NULL;

  char* fname = basename(dir->glob_res.gl_pathv[i]);
  strcpy(dir->_d_dirent.d_name, fname);

  dir->_d_dirent.d_attrib = 0;

  struct stat file_stat;
  stat(dir->glob_res.gl_pathv[i], &file_stat);
  dir->_d_dirent.d_size = file_stat.st_size;
  dir->_d_dirent.d_wr_date = 0;
  dir->_d_dirent.d_wr_time = 0;
  if((file_stat.st_mode & S_IFMT) == S_IFDIR) {
    dir->_d_dirent.d_attrib = _A_SUBDIR;
  }

  dir->glob_curr_entry++;

  return &dir->_d_reserved;
}

int closedir1(DIR * dir)
{
   printf("closedir\n");

   if( dir==NULL ) {
     errno = EBADF;
     return -1;
   }
   globfree(&dir->glob_res);
   free(dir->dirname);
   free(dir);
   return 0;
}
