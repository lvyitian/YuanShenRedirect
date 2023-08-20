//
// Created by 53548 on 8/20/2023.
//

#include "open_hook.h"
#include <bytehook.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

// You must free the result if result is non-NULL.
char* str_replace(char* orig, char* rep, char* with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
char* path_replace(const char* path)
{
    path=str_replace(path,"/data/media/0/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    char* tmp=path;
    path=str_replace(path,"/storage/emulated/0/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/storage/sdcard0/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/storage/self/primary/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/data/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/mnt/user/0/emulated/0/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/mnt/user/0/sdcard0/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/mnt/user/0/self/primary/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/mnt/sdcard/Android/data/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/data/user/0/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    tmp=path;
    path=str_replace(path,"/data/user_de/0/com.miHoYo.Yuanshen/files/VideoAssets","/data/sdext2/ys/VideoAssets");
    if(tmp) free(tmp);
    return path;
}
int statvfs_hook(const char* path, void* status)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(statvfs_hook, statvfs_t, path,status);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int ftw_hook(const char* path,ftw_func_t func,int ndirs)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(ftw_hook, ftw_t, path,func,ndirs);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int lstat_hook(const char* path,void* size)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(lstat_hook, lstat_t, path,size);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int stat_hook(const char* path,void* size)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(stat_hook, stat_t, path,size);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
ssize_t readlink_hook(const char* path,char* buf,size_t len) {
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(readlink_hook, readlink_t, path,buf,len);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}

int symlink_hook(const char* old, const char* new)
{
    old=path_replace(old);
    new=path_replace(new);
    int res=BYTEHOOK_CALL_PREV(symlink_hook,symlink_t,old,new);
    if(old) free(old);
    if(new) free(new);
    BYTEHOOK_POP_STACK();
    return res;
}
int link_hook(const char* old, const char* new)
{
    old=path_replace(old);
    new=path_replace(new);
    int res=BYTEHOOK_CALL_PREV(link_hook,link_t,old,new);
    if(old) free(old);
    if(new) free(new);
    BYTEHOOK_POP_STACK();
    return res;
}
int mkdir_hook(const char* path,mode_t mode)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(mkdir_hook, mkdir_t, path,mode);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int unlink_hook(const char* path)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(unlink_hook, unlink_t, path);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int rmdir_hook(const char* path)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(rmdir_hook, rmdir_t, path);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int remove_hook(const char* path)
{
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(remove_hook, remove_t, path);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int create_hook(const char* path,mode_t mode){
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(create_hook, create_t, path,mode);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
int access_hook(const char* path, int mode) {
    path=path_replace(path);
    int res = BYTEHOOK_CALL_PREV(access_hook, access_t, path,mode);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return res;
}
DIR* opendir_hook(const char* path) {
    path=path_replace(path);
    DIR* dir = BYTEHOOK_CALL_PREV(opendir_hook, opendir_t, path);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return dir;
}
int open_hook(const char* path, int flags, mode_t modes) {
    path=path_replace(path);
    int fd = BYTEHOOK_CALL_PREV(open_hook, open_t, path, flags, modes);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return fd;
}
int open_real_hook(const char* path, int flags, mode_t modes) {
    path=path_replace(path);
    int fd = BYTEHOOK_CALL_PREV(open_real_hook, open_real_t, path, flags, modes);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return fd;
}

int open2_hook(const char* path, int flags) {
    path=path_replace(path);
    int fd = BYTEHOOK_CALL_PREV(open2_hook, open2_t, path, flags);
    if(path) free(path);
    BYTEHOOK_POP_STACK();
    return fd;
}
