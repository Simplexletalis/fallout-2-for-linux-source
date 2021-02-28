
#ifndef __FILES_H__
#define __FILES_H__

#ifdef __cplusplus
extern "C" {
#endif

int updateWorkingDir();
const char* getWorkingDir();

char* changePath(char* newname, const char* oldname);

#ifdef __cplusplus
}
#endif

#endif
