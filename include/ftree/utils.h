#ifndef _UTILS_H_
#define _UTILS_H_

#include "cstdio"
#include "cstdlib"
#include "cstring"

char **parse_args(char *buffer, int *argc);

char **parse_path(char *path, int *len);

void split_path(char *path, char *base_path, char *file_path);

void *kmalloc(size_t size);

void kfree(void *ptr);

#endif