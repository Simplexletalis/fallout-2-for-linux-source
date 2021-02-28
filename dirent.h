#ifndef __DIRENT_H__
#define __DIRENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <glob.h>

#define _A_NORMAL       0x00    /* Normal file - read/write permitted */
#define _A_RDONLY       0x01    /* Read-only file */
#define _A_HIDDEN       0x02    /* Hidden file */
#define _A_SYSTEM       0x04    /* System file */
#define _A_VOLID        0x08    /* Volume-ID entry */
#define _A_SUBDIR       0x10    /* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */



#define NAME_MAX   12      /* 8 chars + '.' +  3 chars */

struct __attribute__((packed)) dirent {
   char     d_attrib;            /* attribute byte for matched file */
   unsigned short d_wr_time;           /* time of last write to file */
   unsigned short d_wr_date;           /* date of last write to file */
   long     d_size;              /* size of file */
   char     d_name[NAME_MAX+1];
};

typedef __attribute__((packed)) struct {
   char          _d_reserved[21]; /* Microsoft reserved - do not change */
   struct dirent _d_dirent;
   char *        dirname;
   glob_t glob_res;
   int glob_curr_entry;
} DIR;

DIR            *   opendir1(const char * __dirname);
struct dirent  *   readdir1(DIR * __dir);
int               closedir1(DIR * __dir);

#ifdef __cplusplus
}
#endif


#endif
