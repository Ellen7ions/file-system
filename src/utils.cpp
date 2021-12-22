#include "ftree/utils.h"

char **parse_args(char *buffer, int *argc) {
    return nullptr;
}

char **parse_path(char *path, int *len) {
    char **result = (char **) kmalloc(sizeof(char *));
    char *pointer = strtok(path, "/");
    *len = 0;
    while (pointer != NULL) {
        *(result + *(len)) = pointer;
        *len += 1;
        pointer = strtok(NULL, "/");
    };
    return result;
}

void split_path(char *path, char *base_path, char *file_path) {
    char *p = path + strlen(path) - 1;
    if (*p == '/') p -= 1;

    while (p > path && *p != '/') p -= 1;
    if (p != path) {
        *p = '\0';
        strcpy(base_path, path);
        strcpy(file_path, p + 1);
    } else {
        *base_path = '\0';
        strcpy(file_path, p);
    }
}

void *kmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("Error memory allocation !\n");
        exit(-1);
    }
    return ptr;
}

void kfree(void *ptr) {
    if (ptr == NULL) {
        printf("Error memory free !\n");
        exit(-1);
    }
    free(ptr);
}