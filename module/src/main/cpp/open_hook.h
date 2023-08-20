//
// Created by 53548 on 8/20/2023.
//

#ifndef ZYGISK_YUANSHENREDIRECT_OPEN_HOOK_H
#define ZYGISK_YUANSHENREDIRECT_OPEN_HOOK_H

#include <sys/types.h>
#include <dirent.h>

typedef int (*open_t)(const char *, int, mode_t);
typedef int (*open_real_t)(const char *, int, mode_t);
typedef int (*open2_t)(const char *, int);
typedef DIR* (*opendir_t)(const char*);
typedef int (*access_t)(const char*,int);

int open_hook(const char* path, int flags, mode_t modes);
int open_real_hook(const char* pathname, int flags, mode_t modes);
int open2_hook(const char* pathname, int flags);
char* str_replace(char* orig, char* rep, char* with);
char* path_replace(const char* path);
DIR* opendir_hook(const char* path);
int access_hook(const char* path, int mode);
#endif //ZYGISK_YUANSHENREDIRECT_OPEN_HOOK_H
