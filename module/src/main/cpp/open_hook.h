//
// Created by 53548 on 8/20/2023.
//

#ifndef ZYGISK_YUANSHENREDIRECT_OPEN_HOOK_H
#define ZYGISK_YUANSHENREDIRECT_OPEN_HOOK_H

#include <bytehook.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

typedef int (*open_t)(const char *, int, mode_t);
typedef int (*open_real_t)(const char *, int, mode_t);
typedef int (*open2_t)(const char *, int);
typedef DIR* (*opendir_t)(const char*);
typedef int (*access_t)(const char*,int);
typedef int (*unlink_t)(const char*);
typedef int (*rmdir_t)(const char*);
typedef int (*remove_t)(const char*);
typedef int (*create_t)(const char*,mode_t);
typedef int (*mkdir_t)(const char*,mode_t);
typedef int (*link_t)(const char*,const char*);
typedef int (*symlink_t)(const char*,const char*);
typedef ssize_t (*readlink_t)(const char*,char*,size_t);
typedef int (*stat_t)(const char*,struct stat*);
typedef int (*lstat_t)(const char*,struct stat*);
typedef int (*ftw_func_t)(const char*,const struct stat*,int);
typedef int (*ftw_t)(const char*,ftw_func_t,int);
typedef int (*statvfs_t)(const char*, struct statvfs*);

int open_hook(const char* path, int flags, mode_t modes);
int open_real_hook(const char* pathname, int flags, mode_t modes);
int open2_hook(const char* pathname, int flags);
char* str_replace(char* orig, char* rep, char* with);
char* path_replace(const char* path);
DIR* opendir_hook(const char* path);
int access_hook(const char* path, int mode);
int unlink_hook(const char* path);
int rmdir_hook(const char* path);
int remove_hook(const char* path);
int create_hook(const char* path,mode_t mode);
int mkdir_hook(const char* path,mode_t mode);
int link_hook(const char* old,const char* newpath);
int symlink_hook(const char* old,const char* newpath);
ssize_t readlink_hook(const char* path,char* buf,size_t len);
int stat_hook(const char* path,void* status);
int lstat_hook(const char* path,void* status);
int ftw_hook(const char* path,ftw_func_t func,int ndirs);
int statvfs_hook(const char* path, void* status);

#endif //ZYGISK_YUANSHENREDIRECT_OPEN_HOOK_H
